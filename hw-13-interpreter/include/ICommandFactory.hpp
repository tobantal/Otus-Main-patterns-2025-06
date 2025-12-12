#pragma once

#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>

/**
 * @file ICommandFactory.hpp
 * @brief Интерфейс фабрики команд
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерфейс фабрики для создания команд
 * 
 * Используется для регистрации в IoC контейнере.
 * Позволяет создавать команды с параметрами через IoC,
 * который поддерживает только фабрики без аргументов.
 */
class ICommandFactory {
public:
    virtual ~ICommandFactory() = default;
    
    /**
     * @brief Создает команду
     * 
     * @param object Целевой объект команды
     * @param params Параметры команды
     * @return Указатель на созданную команду
     */
    virtual std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) = 0;
};