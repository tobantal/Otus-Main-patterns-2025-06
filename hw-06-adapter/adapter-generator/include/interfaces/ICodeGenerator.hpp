/**
 * @file ICodeGenerator.hpp
 * @brief Интерфейс генератора кода адаптеров
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include <string>
#include "../models/InterfaceInfo.hpp"

/**
 * @brief Интерфейс для генерации кода адаптеров
 * 
 * Определяет контракт для генераторов, которые создают код адаптеров
 * на основе информации об интерфейсах.
 */
class ICodeGenerator {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~ICodeGenerator() = default;

    /**
     * @brief Генерирует код адаптера для интерфейса
     * @param interface Информация об интерфейсе
     * @return Сгенерированный код адаптера
     * @throws std::runtime_error если генерация не удалась
     */
    virtual std::string generateAdapter(const InterfaceInfo& interface) = 0;

    /**
     * @brief Генерирует код регистрации адаптеров в IoC контейнере
     * @param interfaces Список интерфейсов для регистрации
     * @return Сгенерированный код функции регистрации
     */
    virtual std::string generateRegistrationCode(const std::vector<InterfaceInfo>& interfaces) = 0;
};