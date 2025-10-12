/**
 * @file MethodInfo.hpp
 * @brief Структура для хранения информации о методе интерфейса
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <string>
#include <vector>
#include "CustomImplementation.hpp"

/**
 * @brief Структура для хранения информации о методе интерфейса
 * 
 * Содержит всю необходимую информацию для генерации адаптера:
 * тип возврата, имя метода, параметры, модификаторы и кастомную реализацию.
 */
struct MethodInfo {
    std::string returnType;          ///< Тип возвращаемого значения
    std::string name;               ///< Имя метода
    std::vector<std::string> parameters; ///< Список параметров метода
    bool isConst = false;           ///< Является ли метод константным
    bool isPureVirtual = false;     ///< Является ли метод чисто виртуальным
    CustomImplementation customImpl; ///< Информация о кастомной реализации

    /**
     * @brief Проверяет, является ли метод геттером
     * @return true если метод начинается с "get"
     */
    bool isGetter() const;

    /**
     * @brief Проверяет, является ли метод сеттером
     * @return true если метод начинается с "set" и имеет ровно один параметр
     */
    bool isSetter() const;

    /**
     * @brief Проверяет, является ли метод void методом без параметров
     * @return true если метод возвращает void и не имеет параметров
     */
    bool isVoidMethod() const;

    /**
     * @brief Извлекает имя свойства из имени метода
     * @return Имя свойства (для getPosition возвращает "Position")
     */
    std::string getPropertyName() const;

    /**
     * @brief Проверяет, имеет ли метод кастомную реализацию
     * @return true если указана кастомная реализация
     */
    bool hasCustomImplementation() const;
};