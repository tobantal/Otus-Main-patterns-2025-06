#pragma once

#include "ICommand.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>

/**
 * @file ThreadSafeQueue.hpp
 * @brief Потокобезопасная очередь команд
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Потокобезопасная очередь команд
 *
 * Реализует thread-safe очередь с блокирующими операциями.
 * Использует mutex и condition_variable для синхронизации.
 */
class LegacyThreadSafeQueue {
private:
    std::queue<std::shared_ptr<ICommand>> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condVar_;
    bool shutdown_;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    LegacyThreadSafeQueue();

    /**
     * @brief Деструктор
     */
    ~LegacyThreadSafeQueue() = default;

    /**
     * @brief Добавить команду в очередь
     * @param command Команда для добавления
     */
    void push(std::shared_ptr<ICommand> command);

    /**
     * @brief Извлечь команду из очереди (блокирующий вызов)
     * @param command Указатель для сохранения извлеченной команды
     * @return true если команда извлечена, false если очередь закрыта
     */
    bool pop(std::shared_ptr<ICommand>& command);

    /**
     * @brief Попытаться извлечь команду из очереди (неблокирующий вызов)
     * @param command Указатель для сохранения извлеченной команды
     * @return true если команда извлечена, false если очередь пуста
     */
    bool tryPop(std::shared_ptr<ICommand>& command);

    /**
     * @brief Извлечь команду с таймаутом
     * @param command Указатель для сохранения извлеченной команды
     * @param timeout Максимальное время ожидания
     * @return true если команда извлечена, false если таймаут или закрыта
     */
    bool popWithTimeout(std::shared_ptr<ICommand>& command, 
                        std::chrono::milliseconds timeout);

    /**
     * @brief Проверить, пуста ли очередь
     * @return true если очередь пуста
     */
    bool isEmpty() const;

    /**
     * @brief Получить размер очереди
     * @return Количество команд в очереди
     */
    size_t size() const;

    /**
     * @brief Закрыть очередь (пробудить все ожидающие потоки)
     */
    void shutdown();

    /**
     * @brief Проверить, закрыта ли очередь
     * @return true если очередь закрыта
     */
    bool isShutdown() const;
};