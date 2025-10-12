/**
 * @file CustomImplementation.hpp
 * @brief Структура для хранения информации о кастомной реализации метода
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <string>

/**
 * @brief Структура для хранения информации о кастомной реализации метода
 * 
 * Используется когда метод адаптера требует сложной логики,
 * которая не может быть выражена простым геттером/сеттером.
 */
struct CustomImplementation {
    bool hasCustom = false;         ///< Флаг наличия кастомной реализации
    std::string className;          ///< Имя класса с кастомной реализацией
    std::string methodName;         ///< Имя метода кастомной реализации
    std::string fullReference;     ///< Полная ссылка (ClassName::methodName)

    /**
     * @brief Проверяет, корректно ли заполнена информация о кастомной реализации
     * @return true если все необходимые поля заполнены
     */
    bool isValid() const;

    /**
     * @brief Парсит строку аннотации и заполняет структуру
     * @param annotation Строка аннотации типа "ClassName::methodName"
     * @return true если парсинг успешен
     */
    bool parseFromAnnotation(const std::string& annotation);

    /**
     * @brief Возвращает полную ссылку на кастомную реализацию
     * @return Строка вида "ClassName::methodName"
     */
    std::string getFullReference() const;
};