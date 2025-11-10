#pragma once

#include "IExecutorState.hpp"
#include "ThreadSafeQueue.hpp"
#include <thread>
#include <memory>

/**
 * @file CommandExecutor.hpp
 * @brief Минималистичный исполнитель команд в отдельном потоке
 * @author Anton Tobolkin
 * @version 3.0
 */
class CommandExecutor {
private:
    std::shared_ptr<ThreadSafeQueue> queue_;
    std::shared_ptr<IExecutorState> currentState_;
    std::unique_ptr<std::thread> thread_;

    /**
     * @brief Основной цикл выполнения команд
     */
    void executionLoop();

public:
    /**
     * @brief Конструктор
     * @param queue Очередь команд
     * @param initialState Начальное состояние (nullptr = не запущен)
     */
    CommandExecutor(std::shared_ptr<ThreadSafeQueue> queue,
                    std::shared_ptr<IExecutorState> initialState);

    /**
     * @brief Деструктор — безопасно завершает поток
     */
    ~CommandExecutor();

    /**
     * @brief Запустить поток выполнения
     */
    void start();

    /**
     * @brief Дождаться завершения потока
     */
    void join();

    /**
     * @brief Установить новое состояние (nullptr = остановка)
     */
    void setState(std::shared_ptr<IExecutorState> state);

    /**
     * @brief Получить текущее состояние
     */
    std::shared_ptr<IExecutorState> getState() const;

    /**
     * @brief Получить очередь команд
     */
    std::shared_ptr<ThreadSafeQueue> getQueue() const;
};
