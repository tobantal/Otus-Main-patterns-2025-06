/**
 * @file FileUtils.hpp
 * @brief Утилиты для работы с файлами
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <string>
#include <vector>

/**
 * @brief Класс с утилитарными функциями для работы с файлами
 */
class FileUtils {
public:
    /**
     * @brief Читает содержимое файла в строку
     * @param filePath Путь к файлу
     * @return Содержимое файла
     * @throws std::runtime_error если файл не удается прочитать
     */
    static std::string readFile(const std::string& filePath);

    /**
     * @brief Записывает строку в файл
     * @param filePath Путь к файлу
     * @param content Содержимое для записи
     * @throws std::runtime_error если файл не удается записать
     */
    static void writeFile(const std::string& filePath, const std::string& content);

    /**
     * @brief Находит все файлы с заданным расширением в директории
     * @param directory Путь к директории
     * @param extension Расширение файлов (например, ".hpp")
     * @param recursive Рекурсивный поиск в поддиректориях
     * @return Вектор путей к найденным файлам
     */
    static std::vector<std::string> findFilesWithExtension(
        const std::string& directory,
        const std::string& extension,
        bool recursive = true);

    /**
     * @brief Проверяет существование файла
     * @param filePath Путь к файлу
     * @return true если файл существует
     */
    static bool fileExists(const std::string& filePath);

    /**
     * @brief Создает директории в пути если они не существуют
     * @param dirPath Путь к директории
     */
    static void createDirectories(const std::string& dirPath);

    /**
     * @brief Извлекает имя файла из полного пути
     * @param filePath Полный путь к файлу
     * @return Имя файла
     */
    static std::string getFileName(const std::string& filePath);

    /**
     * @brief Извлекает расширение файла
     * @param filePath Путь к файлу
     * @return Расширение файла (включая точку)
     */
    static std::string getFileExtension(const std::string& filePath);
};