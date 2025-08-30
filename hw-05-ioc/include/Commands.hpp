#pragma once

#include "ICommand.hpp"
#include "IocContainer.hpp"
#include <string>
#include <memory>
#include <functional>

/**
 * @file Commands.hpp
 * @brief Команды для управления IoC контейнером
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда регистрации зависимости
 * 
 * Выполняет регистрацию новой зависимости в IoC контейнере
 */
class RegisterCommand : public ICommand {
private:
    std::string key_;
    IocContainer::FactoryFunction factory_;
    
public:
    /**
     * @brief Конструктор команды регистрации
     * @param key Ключ зависимости
     * @param factory Фабричная функция
     */
    RegisterCommand(const std::string& key, IocContainer::FactoryFunction factory);
    
    /**
     * @brief Выполняет регистрацию зависимости
     */
    void execute() override;
};

/**
 * @brief Команда создания нового скоупа
 * 
 * Создает новый скоуп с заданным идентификатором
 */
class CreateScopeCommand : public ICommand {
private:
    std::string scopeId_;
    
public:
    /**
     * @brief Конструктор команды создания скоупа
     * @param scopeId Идентификатор скоупа
     */
    explicit CreateScopeCommand(const std::string& scopeId);
    
    /**
     * @brief Выполняет создание скоупа
     */
    void execute() override;
};

/**
 * @brief Команда установки текущего скоупа
 * 
 * Устанавливает указанный скоуп как текущий для данного потока
 */
class SetCurrentScopeCommand : public ICommand {
private:
    std::string scopeId_;
    
public:
    /**
     * @brief Конструктор команды установки скоупа
     * @param scopeId Идентификатор скоупа
     */
    explicit SetCurrentScopeCommand(const std::string& scopeId);
    
    /**
     * @brief Выполняет установку текущего скоупа
     */
    void execute() override;
};