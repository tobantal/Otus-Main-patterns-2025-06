/**
 * @file TestFileUtils.cpp
 * @brief Unit-тесты для FileUtils
 * @author Anton Tobolkin
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../include/utils/FileUtils.hpp"

/**
 * @brief Тестовый класс для FileUtils
 */
class FileUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную директорию для тестов
        testDir = std::filesystem::temp_directory_path() / "fileutils_test";
        std::filesystem::create_directories(testDir);
    }

    void TearDown() override {
        // Очищаем временные файлы
        std::filesystem::remove_all(testDir);
    }

    std::filesystem::path testDir;
};

/**
 * @brief Тест чтения и записи файлов
 */
TEST_F(FileUtilsTest, ReadWriteFileTest) {
    std::string testFilePath = (testDir / "test_file.txt").string();
    std::string testContent = R"(
        This is a test content.
        It contains multiple lines.
        And special characters: !@#$%^&*()
        Unicode: αβγδε
    )";

    // Тест записи файла
    EXPECT_NO_THROW(FileUtils::writeFile(testFilePath, testContent));

    // Проверяем, что файл существует
    EXPECT_TRUE(FileUtils::fileExists(testFilePath));

    // Тест чтения файла
    std::string readContent;
    EXPECT_NO_THROW(readContent = FileUtils::readFile(testFilePath));

    // Проверяем, что содержимое совпадает
    EXPECT_EQ(readContent, testContent);
}

/**
 * @brief Тест чтения несуществующего файла
 */
TEST_F(FileUtilsTest, ReadNonExistentFileTest) {
    std::string nonExistentFile = (testDir / "non_existent.txt").string();
    
    EXPECT_THROW(FileUtils::readFile(nonExistentFile), std::runtime_error);
}

/**
 * @brief Тест записи в недоступную директорию
 */
TEST_F(FileUtilsTest, WriteToInaccessibleDirectoryTest) {
    // Пытаемся записать в несуществующую директорию
    std::string inaccessiblePath = (testDir / "non_existent_dir" / "file.txt").string();
    
    EXPECT_THROW(FileUtils::writeFile(inaccessiblePath, "test content"), std::runtime_error);
}

/**
 * @brief Тест поиска файлов с расширением
 */
TEST_F(FileUtilsTest, FindFilesWithExtensionTest) {
    // Создаем тестовые файлы
    std::vector<std::string> testFiles = {
        "test1.hpp", "test2.cpp", "test3.hpp", "test4.h", 
        "readme.txt", "config.hpp", "implementation.cpp"
    };

    for (const auto& fileName : testFiles) {
        std::string filePath = (testDir / fileName).string();
        FileUtils::writeFile(filePath, "// Test content");
    }

    // Создаем поддиректорию с дополнительными файлами
    std::filesystem::path subdir = testDir / "subdir";
    std::filesystem::create_directories(subdir);
    
    std::vector<std::string> subdirFiles = {
        "subtest1.hpp", "subtest2.cpp", "subtest3.h"
    };

    for (const auto& fileName : subdirFiles) {
        std::string filePath = (subdir / fileName).string();
        FileUtils::writeFile(filePath, "// Subdir test content");
    }

    // Тест поиска .hpp файлов (рекурсивно)
    auto hppFiles = FileUtils::findFilesWithExtension(testDir.string(), ".hpp", true);
    EXPECT_EQ(hppFiles.size(), 4); // test1.hpp, test3.hpp, config.hpp, subtest1.hpp

    // Тест поиска .cpp файлов (рекурсивно)
    auto cppFiles = FileUtils::findFilesWithExtension(testDir.string(), ".cpp", true);
    EXPECT_EQ(cppFiles.size(), 3); // test2.cpp, implementation.cpp, subtest2.cpp

    // Тест поиска .h файлов (рекурсивно)
    auto hFiles = FileUtils::findFilesWithExtension(testDir.string(), ".h", true);
    EXPECT_EQ(hFiles.size(), 2); // test4.h, subtest3.h

    // Тест поиска .hpp файлов (не рекурсивно)
    auto hppFilesNonRecursive = FileUtils::findFilesWithExtension(testDir.string(), ".hpp", false);
    EXPECT_EQ(hppFilesNonRecursive.size(), 3); // test1.hpp, test3.hpp, config.hpp

    // Проверяем, что возвращаются правильные пути
    for (const auto& file : hppFiles) {
        EXPECT_TRUE(file.size() >= 4 && file.substr(file.size() - 4) == ".hpp");
        EXPECT_TRUE(FileUtils::fileExists(file));
    }
}

/**
 * @brief Тест поиска файлов в пустой директории
 */
TEST_F(FileUtilsTest, FindFilesInEmptyDirectoryTest) {
    std::filesystem::path emptyDir = testDir / "empty";
    std::filesystem::create_directories(emptyDir);

    auto files = FileUtils::findFilesWithExtension(emptyDir.string(), ".hpp");
    EXPECT_TRUE(files.empty());
}

/**
 * @brief Тест поиска файлов в несуществующей директории
 */
TEST_F(FileUtilsTest, FindFilesInNonExistentDirectoryTest) {
    std::string nonExistentDir = (testDir / "non_existent").string();
    EXPECT_THROW(
        FileUtils::findFilesWithExtension(nonExistentDir, ".hpp"),
        std::filesystem::filesystem_error
    );
}

/**
 * @brief Тест проверки существования файла
 */
TEST_F(FileUtilsTest, FileExistsTest) {
    std::string existingFile = (testDir / "existing.txt").string();
    std::string nonExistentFile = (testDir / "non_existent.txt").string();

    // Создаем файл
    FileUtils::writeFile(existingFile, "test");

    EXPECT_TRUE(FileUtils::fileExists(existingFile));
    EXPECT_FALSE(FileUtils::fileExists(nonExistentFile));
    
    // Тест с директорией
    EXPECT_TRUE(FileUtils::fileExists(testDir.string()));
}

/**
 * @brief Тест создания директорий
 */
TEST_F(FileUtilsTest, CreateDirectoriesTest) {
    std::filesystem::path deepPath = testDir / "level1" / "level2" / "level3";
    std::string deepPathStr = deepPath.string();

    // Директория не должна существовать
    EXPECT_FALSE(std::filesystem::exists(deepPath));

    // Создаем директории
    EXPECT_NO_THROW(FileUtils::createDirectories(deepPathStr));

    // Проверяем, что директория создана
    EXPECT_TRUE(std::filesystem::exists(deepPath));
    EXPECT_TRUE(std::filesystem::is_directory(deepPath));
}

/**
 * @brief Тест создания уже существующих директорий
 */
TEST_F(FileUtilsTest, CreateExistingDirectoriesTest) {
    // Пытаемся создать уже существующую директорию
    EXPECT_NO_THROW(FileUtils::createDirectories(testDir.string()));
    
    // Директория должна по-прежнему существовать
    EXPECT_TRUE(std::filesystem::exists(testDir));
}

/**
 * @brief Тест извлечения имени файла
 */
TEST_F(FileUtilsTest, GetFileNameTest) {
    EXPECT_EQ(FileUtils::getFileName("/path/to/file.txt"), "file.txt");
    EXPECT_EQ(FileUtils::getFileName("C:\\Windows\\System32\\file.exe"), "file.exe");
    EXPECT_EQ(FileUtils::getFileName("simple_file.hpp"), "simple_file.hpp");
    EXPECT_EQ(FileUtils::getFileName("/path/to/directory/"), "");
    EXPECT_EQ(FileUtils::getFileName(""), "");
    EXPECT_EQ(FileUtils::getFileName("file_without_extension"), "file_without_extension");
}

/**
 * @brief Тест извлечения расширения файла
 */
TEST_F(FileUtilsTest, GetFileExtensionTest) {
    EXPECT_EQ(FileUtils::getFileExtension("file.txt"), ".txt");
    EXPECT_EQ(FileUtils::getFileExtension("archive.tar.gz"), ".gz");
    EXPECT_EQ(FileUtils::getFileExtension("file.HPP"), ".HPP");
    EXPECT_EQ(FileUtils::getFileExtension("file_without_extension"), "");
    EXPECT_EQ(FileUtils::getFileExtension(""), "");
    EXPECT_EQ(FileUtils::getFileExtension(".hidden_file"), "");
    EXPECT_EQ(FileUtils::getFileExtension("path/to/.hidden_file.txt"), ".txt");
}

/**
 * @brief Тест работы с пустыми файлами
 */
TEST_F(FileUtilsTest, EmptyFileTest) {
    std::string emptyFilePath = (testDir / "empty.txt").string();

    // Записываем пустое содержимое
    FileUtils::writeFile(emptyFilePath, "");

    // Проверяем, что файл существует
    EXPECT_TRUE(FileUtils::fileExists(emptyFilePath));

    // Читаем пустое содержимое
    std::string content = FileUtils::readFile(emptyFilePath);
    EXPECT_TRUE(content.empty());
}

/**
 * @brief Тест работы с большими файлами
 */
TEST_F(FileUtilsTest, LargeFileTest) {
    std::string largeFilePath = (testDir / "large.txt").string();

    // Создаем содержимое размером около 1MB
    std::string largeContent;
    largeContent.reserve(1024 * 1024);
    for (int i = 0; i < 10240; ++i) {
        largeContent += "This is line " + std::to_string(i) + " with some additional content to make it longer.\n";
    }

    // Записываем большой файл
    EXPECT_NO_THROW(FileUtils::writeFile(largeFilePath, largeContent));

    // Читаем большой файл
    std::string readContent;
    EXPECT_NO_THROW(readContent = FileUtils::readFile(largeFilePath));

    // Проверяем, что содержимое совпадает
    EXPECT_EQ(readContent, largeContent);
    EXPECT_GT(readContent.size(), 500000); // Проверяем, что файл действительно большой
}

/**
 * @brief Тест работы с бинарными файлами
 */
TEST_F(FileUtilsTest, BinaryFileTest) {
    std::string binaryFilePath = (testDir / "binary.bin").string();

    // Создаем бинарное содержимое
    std::string binaryContent;
    for (int i = 0; i < 256; ++i) {
        binaryContent.push_back(static_cast<char>(i));
    }

    // Записываем бинарный файл
    EXPECT_NO_THROW(FileUtils::writeFile(binaryFilePath, binaryContent));

    // Читаем бинарный файл
    std::string readContent = FileUtils::readFile(binaryFilePath);

    // Проверяем, что содержимое совпадает
    EXPECT_EQ(readContent.size(), 256);
    for (int i = 0; i < 256; ++i) {
        EXPECT_EQ(static_cast<unsigned char>(readContent[i]), i);
    }
}

/**
 * @brief Тест работы с файлами, содержащими Unicode
 */
TEST_F(FileUtilsTest, UnicodeFileTest) {
    std::string unicodeFilePath = (testDir / "unicode.txt").string();
    std::string unicodeContent = u8"Hello, 世界! Привет, мир! Здравей, свят! αβγδε 🚀🌟💫";

    // Записываем Unicode содержимое
    EXPECT_NO_THROW(FileUtils::writeFile(unicodeFilePath, unicodeContent));

    // Читаем Unicode содержимое
    std::string readContent = FileUtils::readFile(unicodeFilePath);

    // Проверяем, что содержимое совпадает
    EXPECT_EQ(readContent, unicodeContent);
}

/**
 * @brief Тест производительности файловых операций
 */
TEST_F(FileUtilsTest, PerformanceTest) {
    const int fileCount = 100;
    std::vector<std::string> filePaths;

    // Создаем много файлов
    auto startWrite = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < fileCount; ++i) {
        std::string filePath = (testDir / ("perf_test_" + std::to_string(i) + ".txt")).string();
        std::string content = "Performance test content for file " + std::to_string(i);
        
        FileUtils::writeFile(filePath, content);
        filePaths.push_back(filePath);
    }

    auto endWrite = std::chrono::high_resolution_clock::now();

    // Читаем все файлы
    auto startRead = std::chrono::high_resolution_clock::now();

    for (const auto& filePath : filePaths) {
        std::string content = FileUtils::readFile(filePath);
        EXPECT_FALSE(content.empty());
    }

    auto endRead = std::chrono::high_resolution_clock::now();

    // Измеряем время операций
    auto writeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endWrite - startWrite);
    auto readDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endRead - startRead);

    std::cout << "Performance test results:" << std::endl;
    std::cout << "  Writing " << fileCount << " files: " << writeDuration.count() << "ms" << std::endl;
    std::cout << "  Reading " << fileCount << " files: " << readDuration.count() << "ms" << std::endl;

    // Операции не должны занимать слишком много времени
    EXPECT_LT(writeDuration.count(), 5000); // Менее 5 секунд
    EXPECT_LT(readDuration.count(), 5000);  // Менее 5 секунд
}