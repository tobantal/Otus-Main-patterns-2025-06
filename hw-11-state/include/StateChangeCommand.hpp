#pragma once

#include "ICommand.hpp"
#include "IExecutorState.hpp"
#include <memory>

// Forward declaration
class CommandExecutor;

/**
 * @file StateChangeCommand.hpp
 * @brief Базовый класс для команд смены состояния
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Базовый класс для команд, меняющих состояние исполнителя
 * 
 * Инкапсулирует общую логику смены состояния через вызов
 * executor->setState(). Конкретные команды наследуются от этого
 * класса и передают нужное состояние в конструктор.
 */
class StateChangeCommand : public ICommand {
protected:
    /**
     * @brief Исполнитель, чье состояние будет изменено
     */
    std::shared_ptr<CommandExecutor> executor_;
    
    /**
     * @brief Целевое состояние для установки
     */
    std::shared_ptr<IExecutorState> targetState_;
    
public:
    /**
     * @brief Конструктор
     * 
     * @param executor Исполнитель команд
     * @param targetState Новое состояние (nullptr для остановки)
     * @throws std::invalid_argument если executor равен nullptr
     */
    StateChangeCommand(std::shared_ptr<CommandExecutor> executor,
                       std::shared_ptr<IExecutorState> targetState);
    
    /**
     * @brief Деструктор
     */
    ~StateChangeCommand() override = default;
    
    /**
     * @brief Выполнить команду - установить новое состояние
     * 
     * Вызывает executor->setState(targetState_)
     */
    void execute() override;
    
    /**
     * @brief Получить целевое состояние
     * 
     * @return Указатель на целевое состояние
     */
    std::shared_ptr<IExecutorState> getTargetState() const;
};
