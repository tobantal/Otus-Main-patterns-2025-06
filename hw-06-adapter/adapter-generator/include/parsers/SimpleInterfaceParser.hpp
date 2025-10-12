/**
 * @file SimpleInterfaceParser.hpp
 * @brief Простая реализация парсера C++ интерфейсов
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include "../interfaces/IInterfaceParser.hpp"
#include <regex>
#include <map>

/**
 * @brief Простая реализация парсера C++ интерфейсов на основе регулярных выражений
 * 
 * Использует регулярные выражения для поиска классов с чисто виртуальными методами.
 * Поддерживает парсинг аннотаций для кастомных реализаций методов.
 * 
 * @note Для продакшн использования рекомендуется использовать более мощные инструменты
 *       такие как libclang или clang-ast-matchers
 */
class SimpleInterfaceParser : public IInterfaceParser {
private:
    std::regex classRegex_;         ///< Регулярное выражение для поиска классов
    std::regex methodRegex_;        ///< Регулярное выражение для поиска методов
    std::regex namespaceRegex_;     ///< Регулярное выражение для поиска namespace
    std::regex customImplRegex_;    ///< Регулярное выражение для поиска аннотаций

public:
    /**
     * @brief Конструктор, инициализирует регулярные выражения
     */
    SimpleInterfaceParser();

    /**
     * @brief Деструктор
     */
    ~SimpleInterfaceParser() override = default;

    /**
     * @brief Парсит файл и извлекает информацию об интерфейсах
     * @param filePath Путь к файлу для парсинга
     * @return Вектор структур InterfaceInfo с информацией об найденных интерфейсах
     * @throws std::runtime_error если файл не удается прочитать
     */
    std::vector<InterfaceInfo> parseFile(const std::string& filePath) override;

    /**
     * @brief Парсит содержимое строки и извлекает информацию об интерфейсах
     * @param content Содержимое для парсинга
     * @return Вектор структур InterfaceInfo с информацией об найденных интерфейсах
     */
    std::vector<InterfaceInfo> parseContent(const std::string& content) override;

private:
    /**
     * @brief Парсит методы внутри класса
     * @param classContent Содержимое класса между фигурными скобками
     * @param interfaceName Имя интерфейса для формирования ключей IoC
     * @return Вектор информации о методах
     */
    std::vector<MethodInfo> parseMethods(const std::string& classContent, 
                                       const std::string& interfaceName);

    /**
     * @brief Парсит параметры метода
     * @param paramStr Строка с параметрами метода
     * @return Вектор строк с параметрами
     */
    std::vector<std::string> parseParameters(const std::string& paramStr);

    /**
     * @brief Проверяет, что все методы класса являются чисто виртуальными
     * @param methods Вектор методов для проверки
     * @return true если все методы чисто виртуальные
     */
    bool hasOnlyPureVirtual(const std::vector<MethodInfo>& methods);

    /**
     * @brief Извлекает имя namespace из содержимого файла
     * @param content Содержимое файла
     * @param pos Позиция, до которой искать namespace
     * @return Имя namespace или пустая строка
     */
    std::string extractNamespace(const std::string& content, size_t pos);

    /**
     * @brief Находит соответствующую закрывающую скобку
     * @param content Содержимое для поиска
     * @param start Позиция открывающей скобки
     * @return Позиция закрывающей скобки
     */
    size_t findMatchingBrace(const std::string& content, size_t start);

    /**
     * @brief Парсит аннотацию кастомной реализации из комментария
     * @param commentContent Содержимое комментария
     * @return Структура CustomImplementation
     */
    CustomImplementation parseCustomImplementation(const std::string& commentContent);

    /**
     * @brief Извлекает комментарии перед методом
     * @param content Содержимое класса
     * @param methodPos Позиция метода
     * @return Текст комментариев
     */
    std::string extractMethodComments(const std::string& content, size_t methodPos);

    /**
     * @brief Удаляет лишние пробелы из строки
     * @param str Строка для обработки
     * @return Обработанная строка
     */
    std::string trim(const std::string& str);
};