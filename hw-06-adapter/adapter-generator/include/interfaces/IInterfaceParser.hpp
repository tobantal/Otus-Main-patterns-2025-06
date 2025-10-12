/**
 * @file IInterfaceParser.hpp
 * @brief Интерфейс парсера C++ интерфейсов
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <vector>
#include <string>
#include "../models/InterfaceInfo.hpp"

/**
 * @brief Интерфейс для парсинга C++ файлов и извлечения информации об интерфейсах
 * 
 * Определяет контракт для различных типов парсеров, которые могут анализировать
 * исходный код C++ и извлекать информацию о чисто виртуальных классах (интерфейсах).
 */
class IInterfaceParser {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IInterfaceParser() = default;

    /**
     * @brief Парсит файл и извлекает информацию об интерфейсах
     * @param filePath Путь к файлу для парсинга
     * @return Вектор структур InterfaceInfo с информацией об найденных интерфейсах
     * @throws std::runtime_error если файл не удается прочитать или парсинг не удался
     */
    virtual std::vector<InterfaceInfo> parseFile(const std::string& filePath) = 0;

    /**
     * @brief Парсит содержимое строки и извлекает информацию об интерфейсах
     * @param content Содержимое для парсинга
     * @return Вектор структур InterfaceInfo с информацией об найденных интерфейсах
     * @throws std::runtime_error если парсинг не удался
     */
    virtual std::vector<InterfaceInfo> parseContent(const std::string& content) = 0;
};