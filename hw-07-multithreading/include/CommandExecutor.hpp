#pragma once

#include "ThreadSafeQueue.hpp"
#include "ICommand.hpp"
#include <thread>
#include <atomic>
#include <memory>
#include <functional>

/**
 * @file CommandExecutor.hpp
 * @brief Исполнитель команд в отдельном потоке
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Исполнитель команд в отдельном потоке
 *
 * Выполняет команды из потокобезопасной очереди в отдельном потоке.
 * Поддерживает hard и soft stop.
 */
class CommandExecutor {
private:
    std::shared_ptr<ThreadSafeQueue> queue_;
    std::unique_ptr<std::thread> thread_;
    std::atomic<bool> running_;
    std::atomic<bool> softStop_;
    
    /**
     * @brief Основной цикл выполнения команд
     */
    void executionLoop();

public:
    /**
     * @brief Конструктор
     * @param queue Потокобезопасная очередь команд
     */
    explicit CommandExecutor(std::shared_ptr<ThreadSafeQueue> queue);

    /**
     * @brief Деструктор
     */
    ~CommandExecutor();

    // Запретить копирование и присваивание
    CommandExecutor(const CommandExecutor&) = delete;
    CommandExecutor& operator=(const CommandExecutor&) = delete;

    /**
     * @brief Запустить поток выполнения команд
     * @throws std::runtime_error если поток уже запущен
     */
    void start();

    /**
     * @brief Жесткая остановка (hard stop)
     * 
     * Останавливает выполнение немедленно, не дожидаясь завершения команд
     */
    void hardStop();

    /**
     * @brief Мягкая остановка (soft stop)
     * 
     * Останавливает выполнение после обработки всех команд в очереди
     */
    void softStop();

    /**
     * @brief Проверить, запущен ли поток
     * @return true если поток запущен
     */
    bool isRunning() const;

    /**
     * @brief Дождаться завершения потока
     */
    void join();

    /**
     * @brief Получить очередь команд
     * @return Указатель на очередь команд
     */
    std::shared_ptr<ThreadSafeQueue> getQueue() const;
};