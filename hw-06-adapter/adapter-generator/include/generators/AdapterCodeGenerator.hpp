/**
 * @file AdapterCodeGenerator.hpp
 * @brief Генератор кода адаптеров на C++
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include "../interfaces/ICodeGenerator.hpp"
#include <map>

/**
 * @brief Генератор кода адаптеров на C++
 * 
 * Создает код адаптеров паттерна Adapter для интерфейсов,
 * используя шаблоны кода и подстановку переменных.
 * Поддерживает кастомные реализации методов через аннотации.
 */
class AdapterCodeGenerator : public ICodeGenerator {
private:
    static const std::string ADAPTER_TEMPLATE;           ///< Шаблон класса адаптера
    static const std::string GETTER_METHOD_TEMPLATE;     ///< Шаблон геттер метода
    static const std::string SETTER_METHOD_TEMPLATE;     ///< Шаблон сеттер метода
    static const std::string VOID_METHOD_TEMPLATE;       ///< Шаблон void метода
    static const std::string CUSTOM_METHOD_TEMPLATE;     ///< Шаблон кастомного метода
    static const std::string REGISTRATION_TEMPLATE;      ///< Шаблон регистрации

    std::map<std::string, std::string> variables_;       ///< Переменные для подстановки

public:
    /**
     * @brief Конструктор
     */
    AdapterCodeGenerator() = default;

    /**
     * @brief Деструктор
     */
    ~AdapterCodeGenerator() override = default;

    /**
     * @brief Генерирует код адаптера для интерфейса
     * @param interface Информация об интерфейсе
     * @return Сгенерированный код адаптера
     * @throws std::runtime_error если генерация не удалась
     */
    std::string generateAdapter(const InterfaceInfo& interface) override;

    /**
     * @brief Генерирует код регистрации адаптеров в IoC контейнере
     * @param interfaces Список интерфейсов для регистрации
     * @return Сгенерированный код функции регистрации
     */
    std::string generateRegistrationCode(const std::vector<InterfaceInfo>& interfaces) override;

private:
    /**
     * @brief Строит полное имя интерфейса с namespace
     * @param interface Информация об интерфейсе
     * @return Полное имя интерфейса
     */
    std::string buildFullName(const InterfaceInfo& interface);

    /**
     * @brief Генерирует код метода на основе его типа
     * @param interface Информация об интерфейсе
     * @param method Информация о методе
     * @return Сгенерированный код метода
     */
    std::string generateMethod(const InterfaceInfo& interface, const MethodInfo& method);

    /**
     * @brief Генерирует геттер метод
     * @param interface Информация об интерфейсе
     * @param method Информация о методе
     * @return Сгенерированный код геттера
     */
    std::string generateGetter(const InterfaceInfo& interface, const MethodInfo& method);

    /**
     * @brief Генерирует сеттер метод
     * @param interface Информация об интерфейсе
     * @param method Информация о методе
     * @return Сгенерированный код сеттера
     */
    std::string generateSetter(const InterfaceInfo& interface, const MethodInfo& method);

    /**
     * @brief Генерирует void метод
     * @param interface Информация об интерфейсе
     * @param method Информация о методе
     * @return Сгенерированный код void метода
     */
    std::string generateVoidMethod(const InterfaceInfo& interface, const MethodInfo& method);

    /**
     * @brief Генерирует метод с кастомной реализацией
     * @param interface Информация об интерфейсе
     * @param method Информация о методе
     * @return Сгенерированный код кастомного метода
     */
    std::string generateCustomMethod(const InterfaceInfo& interface, const MethodInfo& method);

    /**
     * @brief Извлекает тип параметра из строки параметра
     * @param param Строка параметра
     * @return Тип параметра
     */
    std::string extractParamType(const std::string& param);

    /**
     * @brief Выполняет подстановку переменных в шаблон
     * @param templateStr Строка шаблона
     * @param vars Переменные для подстановки
     * @return Результат подстановки
     */
    std::string renderTemplate(const std::string& templateStr,
                             const std::map<std::string, std::string>& vars);

    /**
     * @brief Генерирует include директивы для интерфейсов
     * @param interfaces Список интерфейсов
     * @return Строка с include директивами
     */
    std::string generateIncludes(const std::vector<InterfaceInfo>& interfaces);
};