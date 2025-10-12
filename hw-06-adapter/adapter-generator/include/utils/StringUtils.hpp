/**
 * @file StringUtils.hpp
 * @brief Утилиты для работы со строками
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <string>
#include <vector>

/**
 * @brief Класс с утилитарными функциями для работы со строками
 */
class StringUtils {
public:
    /**
     * @brief Удаляет пробелы в начале и конце строки
     * @param str Строка для обработки
     * @return Обработанная строка
     */
    static std::string trim(const std::string& str);

    /**
     * FIXME: deprecated method, delete after refactoring
     * @brief Разделяет строку по разделителю
     * @param str Строка для разделения
     * @param delimiter Разделитель
     * @return Вектор подстрок
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Разделяет строку по разделителю
     * @param str Строка для разделения
     * @param delimiter Разделитель
     * @return Вектор подстрок
     */
    static std::vector<std::string> split(const std::string &str, const std::string &delimiter);

    /**
     * @brief Заменяет все вхождения подстроки в строке
     * @param str Исходная строка
     * @param from Заменяемая подстрока
     * @param to Заменяющая подстрока
     * @return Строка с выполненными заменами
     */
    static std::string replaceAll(const std::string& str, 
                                 const std::string& from, 
                                 const std::string& to);

    /**
     * @brief Проверяет, начинается ли строка с префикса
     * @param str Проверяемая строка
     * @param prefix Префикс
     * @return true если строка начинается с префикса
     */
    static bool startsWith(const std::string& str, const std::string& prefix);

    /**
     * @brief Проверяет, заканчивается ли строка суффиксом
     * @param str Проверяемая строка
     * @param suffix Суффикс
     * @return true если строка заканчивается суффиксом
     */
    static bool endsWith(const std::string& str, const std::string& suffix);

    /**
     * @brief Преобразует первую букву в верхний регистр
     * @param str Строка для преобразования
     * @return Строка с заглавной первой буквой
     */
    static std::string capitalize(const std::string& str);

    /**
     * @brief Преобразует строку в snake_case
     * @param camelCase Строка в camelCase
     * @return Строка в snake_case
     */
    static std::string toSnakeCase(const std::string& camelCase);
};