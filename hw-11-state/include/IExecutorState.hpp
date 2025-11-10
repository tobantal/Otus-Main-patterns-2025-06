#pragma once

#include "ICommand.hpp"
#include <memory>

/**
 * @file IExecutorState.hpp
 * @brief Интерфейс состояния исполнителя команд
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерфейс состояния для паттерна Состояние
 * 
 * Определяет контракт для различных режимов обработки команд.
 * Каждое состояние определяет свою логику обработки команды.
 */
class IExecutorState {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IExecutorState() = default;
    
    /**
     * @brief Обработать команду в текущем состоянии
     * 
     * @param command Команда для обработки
     */
    virtual void handle(std::shared_ptr<ICommand> command) = 0;
};
