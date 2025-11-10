#pragma once

#include "IExecutorState.hpp"

/**
 * @file NormalState.hpp
 * @brief Обычное состояние обработки команд
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Обычное состояние обработки команд
 * 
 * В этом состоянии команды извлекаются из очереди и выполняются.
 * Является состоянием по умолчанию для CommandExecutor.
 */
class NormalState : public IExecutorState {
public:
    /**
     * @brief Конструктор
     */
    NormalState() = default;
    
    /**
     * @brief Деструктор
     */
    ~NormalState() override = default;
    
    /**
     * @brief Обработать команду - выполняет её
     * 
     * В обычном состоянии команда просто выполняется через execute()
     * 
     * @param command Команда для выполнения
     */
    void handle(std::shared_ptr<ICommand> command) override;
};
