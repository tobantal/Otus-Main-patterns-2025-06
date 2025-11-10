#pragma once

#include "IExecutorState.hpp"
#include "ThreadSafeQueue.hpp"

/**
 * @file MoveToState.hpp
 * @brief Состояние перенаправления команд в другую очередь
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Состояние перенаправления команд
 * 
 * В этом состоянии команды не выполняются, а перенаправляются
 * в другую очередь. Используется для балансировки нагрузки
 * между серверами/потоками.
 */
class MoveToState : public IExecutorState {
private:
    /**
     * @brief Целевая очередь для перенаправления команд
     */
    std::shared_ptr<ThreadSafeQueue> targetQueue_;
    
public:
    /**
     * @brief Конструктор
     * 
     * @param targetQueue Очередь, в которую будут перенаправляться команды
     * @throws std::invalid_argument если targetQueue равен nullptr
     */
    explicit MoveToState(std::shared_ptr<ThreadSafeQueue> targetQueue);
    
    /**
     * @brief Деструктор
     */
    ~MoveToState() override = default;
    
    /**
     * @brief Обработать команду - перенаправляет в другую очередь
     * 
     * В состоянии MoveTo команда не выполняется, а помещается
     * в целевую очередь для обработки другим исполнителем
     * 
     * @param command Команда для перенаправления
     */
    void handle(std::shared_ptr<ICommand> command) override;
    
    /**
     * @brief Получить целевую очередь
     * 
     * @return Указатель на целевую очередь
     */
    std::shared_ptr<ThreadSafeQueue> getTargetQueue() const;
};
