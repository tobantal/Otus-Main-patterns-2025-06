#pragma once

#include "StateChangeCommand.hpp"
#include "ThreadSafeQueue.hpp"

/**
 * @file MoveToCommand.hpp
 * @brief Команда перехода в состояние перенаправления
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда для перехода в состояние MoveTo
 * 
 * При выполнении переводит исполнитель в состояние MoveToState,
 * где все последующие команды будут перенаправляться в указанную
 * очередь вместо выполнения.
 */
class MoveToCommand : public StateChangeCommand {
private:
    /**
     * @brief Очередь для перенаправления команд
     */
    std::shared_ptr<ThreadSafeQueue> targetQueue_;
    
public:
    /**
     * @brief Конструктор
     * 
     * @param executor Исполнитель команд
     * @param targetQueue Очередь, куда будут перенаправляться команды
     * @throws std::invalid_argument если executor или targetQueue равны nullptr
     */
    MoveToCommand(std::shared_ptr<CommandExecutor> executor,
                  std::shared_ptr<ThreadSafeQueue> targetQueue);
    
    /**
     * @brief Деструктор
     */
    ~MoveToCommand() override = default;
    
    /**
     * @brief Получить целевую очередь
     * 
     * @return Указатель на очередь для перенаправления
     */
    std::shared_ptr<ThreadSafeQueue> getTargetQueue() const;
};
