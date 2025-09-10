/**
 * @file main.cpp
 * @brief Основное приложение генератора адаптеров
 * @author Anton Tobolkin
 * @version 1.0
 */
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "include/parsers/SimpleInterfaceParser.hpp"
#include "include/generators/AdapterCodeGenerator.hpp"
#include "include/utils/FileUtils.hpp"
#include <set>

/**
 * @brief Основной класс генератора адаптеров
 * 
 * Координирует работу парсера и генератора для создания
 * адаптеров интерфейсов C++.
 */
class AdapterGenerator {
private:
    std::unique_ptr<IInterfaceParser> parser_;     ///< Парсер интерфейсов
    std::unique_ptr<ICodeGenerator> generator_;    ///< Генератор кода

public:
    /**
     * @brief Конструктор
     */
    AdapterGenerator() 
        : parser_(std::make_unique<SimpleInterfaceParser>()),
          generator_(std::make_unique<AdapterCodeGenerator>()) {
    }

    /**
     * @brief Генерирует адаптеры для всех интерфейсов в директории
     * @param inputDir Директория с заголовочными файлами
     * @param outputDir Директория для сохранения сгенерированных файлов
     * @param generateSeparateFiles Генерировать отдельные файлы для каждого адаптера
     */
    void generateAdapters(const std::string& inputDir, 
                         const std::string& outputDir,
                         bool generateSeparateFiles = true) {
        
        std::cout << "Scanning directory: " << inputDir << std::endl;

        // Находим все заголовочные файлы
        auto hppFiles = FileUtils::findFilesWithExtension(inputDir, ".hpp", true);
        auto hFiles = FileUtils::findFilesWithExtension(inputDir, ".h", true);
        
        // Объединяем списки файлов
        hppFiles.insert(hppFiles.end(), hFiles.begin(), hFiles.end());

        std::cout << "Found " << hppFiles.size() << " header files" << std::endl;

        // Парсим все файлы и собираем интерфейсы
        std::vector<InterfaceInfo> allInterfaces;
        int filesParsed = 0;
        
        for (const auto& file : hppFiles) {
            try {
                std::cout << "Parsing: " << file << std::endl;
                auto interfaces = parser_->parseFile(file);
                allInterfaces.insert(allInterfaces.end(), 
                                   interfaces.begin(), interfaces.end());
                filesParsed++;
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to parse " << file 
                          << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Parsed " << filesParsed << " files successfully" << std::endl;
        std::cout << "Found " << allInterfaces.size() << " interfaces" << std::endl;

        if (allInterfaces.empty()) {
            std::cout << "No interfaces found. Nothing to generate." << std::endl;
            return;
        }

        // Создаем выходную директорию
        FileUtils::createDirectories(outputDir);

        if (generateSeparateFiles) {
            // Генерируем отдельные файлы для каждого адаптера
            generateSeparateAdapterFiles(allInterfaces, outputDir);
        } else {
            // Генерируем один общий файл
            generateSingleFile(allInterfaces, outputDir);
        }

        // Генерируем файл регистрации
        generateRegistrationFile(allInterfaces, outputDir);

        std::cout << "Generation completed successfully!" << std::endl;
    }

private:
    /**
     * @brief Генерирует отдельные файлы для каждого адаптера
     * @param interfaces Список интерфейсов
     * @param outputDir Выходная директория
     */
    void generateSeparateAdapterFiles(const std::vector<InterfaceInfo>& interfaces,
                                     const std::string& outputDir) {
        
        for (const auto& interface : interfaces) {
            std::string adapterCode = generator_->generateAdapter(interface);
            std::string fileName = interface.getAdapterName() + ".hpp";
            std::string filePath = outputDir + "/" + fileName;
            
            // Добавляем заголовок файла
            std::string fileHeader = "// Auto-generated adapter code\n"
                                   "// DO NOT EDIT MANUALLY\n"
                                   "// Generated for interface: " + interface.getFullName() + "\n\n";
            
            FileUtils::writeFile(filePath, fileHeader + adapterCode);
            std::cout << "Generated: " << fileName << std::endl;
        }
    }

    /**
     * @brief Генерирует один файл со всеми адаптерами
     * @param interfaces Список интерфейсов
     * @param outputDir Выходная директория
     */
    void generateSingleFile(const std::vector<InterfaceInfo>& interfaces,
                           const std::string& outputDir) {
        
        std::string filePath = outputDir + "/GeneratedAdapters.hpp";
        
        std::ostringstream content;
        content << "// Auto-generated adapter code\n";
        content << "// DO NOT EDIT MANUALLY\n";
        content << "// Contains " << interfaces.size() << " adapters\n\n";
        
        content << "#pragma once\n\n";
        
        // Добавляем все необходимые includes
        std::set<std::string> allIncludes;
        for (const auto& interface : interfaces) {
            auto includes = interface.getRequiredIncludes();
            allIncludes.insert(includes.begin(), includes.end());
        }
        
        for (const auto& include : allIncludes) {
            content << "#include \"" << include << "\"\n";
        }
        content << "\n";

        // Генерируем все адаптеры
        for (const auto& interface : interfaces) {
            content << "// ==================== " << interface.className 
                    << " Adapter ====================\n";
            content << generator_->generateAdapter(interface) << "\n\n";
        }

        FileUtils::writeFile(filePath, content.str());
        std::cout << "Generated: GeneratedAdapters.hpp" << std::endl;
    }

    /**
     * @brief Генерирует файл регистрации адаптеров
     * @param interfaces Список интерфейсов
     * @param outputDir Выходная директория
     */
    void generateRegistrationFile(const std::vector<InterfaceInfo>& interfaces,
                                 const std::string& outputDir) {
        
        std::string registrationCode = generator_->generateRegistrationCode(interfaces);
        std::string filePath = outputDir + "/AdapterRegistry.hpp";
        
        std::string fileHeader = "// Auto-generated adapter registration\n"
                               "// DO NOT EDIT MANUALLY\n\n";
        
        FileUtils::writeFile(filePath, fileHeader + registrationCode);
        std::cout << "Generated: AdapterRegistry.hpp" << std::endl;
    }
};

/**
 * @brief Выводит справку по использованию программы
 * @param programName Имя программы
 */
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <input_directory> <output_directory>\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -s, --single-file       Generate single file with all adapters\n";
    std::cout << "  -m, --separate-files    Generate separate files for each adapter (default)\n";
    std::cout << "\nArguments:\n";
    std::cout << "  input_directory         Directory to search for interface files\n";
    std::cout << "  output_directory        Directory to save generated adapters\n";
    std::cout << "\nExample:\n";
    std::cout << "  " << programName << " ./include ./generated\n";
    std::cout << "  " << programName << " -s ./src/interfaces ./output\n";
}

/**
 * @brief Главная функция программы
 * @param argc Количество аргументов
 * @param argv Массив аргументов
 * @return Код возврата
 */
int main(int argc, char* argv[]) {
    std::cout << "=== C++ Interface Adapter Generator ===" << std::endl;
    std::cout << "Version 1.0" << std::endl << std::endl;

    // Парсинг аргументов командной строки
    bool generateSeparateFiles = true;
    std::string inputDir, outputDir;
    
    int argIndex = 1;
    while (argIndex < argc) {
        std::string arg = argv[argIndex];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--single-file") {
            generateSeparateFiles = false;
        } else if (arg == "-m" || arg == "--separate-files") {
            generateSeparateFiles = true;
        } else if (inputDir.empty()) {
            inputDir = arg;
        } else if (outputDir.empty()) {
            outputDir = arg;
        } else {
            std::cerr << "Error: Too many arguments" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        
        argIndex++;
    }

    // Проверка обязательных аргументов
    if (inputDir.empty() || outputDir.empty()) {
        std::cerr << "Error: Input and output directories are required" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // Проверка существования входной директории
    if (!FileUtils::fileExists(inputDir)) {
        std::cerr << "Error: Input directory does not exist: " << inputDir << std::endl;
        return 1;
    }

    try {
        AdapterGenerator generator;
        generator.generateAdapters(inputDir, outputDir, generateSeparateFiles);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}