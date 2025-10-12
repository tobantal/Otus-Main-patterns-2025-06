/**
 * @file InterfaceInfo.hpp
 * @brief Структура для хранения информации об интерфейсе
 * @author Adapter Generator Team
 * @version 1.0
 */

#pragma once

#include <string>
#include <vector>
#include "MethodInfo.hpp"

/**
 * @brief Структура для хранения информации об интерфейсе
 * 
 * Содержит информацию о C++ интерфейсе (чисто виртуальном классе):
 * имя, пространство имен и список методов.
 */
struct InterfaceInfo {
    std::string namespaceName;      ///< Имя пространства имен
    std::string className;          ///< Имя класса интерфейса
    std::vector<MethodInfo> methods; ///< Список методов интерфейса
    std::string sourceFile;         ///< Исходный файл, где определен интерфейс

    /**
     * @brief Получить полное имя интерфейса с пространством имен
     * @return Полное имя (например, "MyNamespace::IMovable")
     */
    std::string getFullName() const;

    /**
     * @brief Получить имя адаптера на основе имени интерфейса
     * @return Имя адаптера (для IMovable возвращает "MovableAdapter")
     */
    std::string getAdapterName() const;

    /**
     * @brief Проверить, является ли класс корректным интерфейсом
     * @return true если класс содержит только чисто виртуальные методы
     */
    bool isValidInterface() const;

    /**
     * @brief Получить список необходимых include файлов
     * @return Вектор строк с именами файлов для включения
     */
    std::vector<std::string> getRequiredIncludes() const;
};