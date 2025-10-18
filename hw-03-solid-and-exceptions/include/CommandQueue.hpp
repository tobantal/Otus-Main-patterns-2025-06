#pragma once

#include <queue>
#include <memory>
#include <map>
#include <typeindex>
#include <functional>
#include "ICommand.hpp"
#include "IExceptionHandler.hpp"

/**
 * @file CommandQueue.hpp
 * @brief Очередь команд и обработка исключений
 * @author Anton Tobolkin
 * @version 1.0
 */
class CommandQueue {
public:
    /**
     * @brief Ключ обработчика: <Тип команды, Тип исключения>
     */
    using HandlerKey = std::pair<std::type_index, std::type_index>;

    /**
     * @brief Конструктор очереди команд
     */
    CommandQueue();

    /**
     * @brief Деструктор очереди команд
     */
    ~CommandQueue() = default;

    /**
     * @brief Добавить команду в очередь
     * @param command Умный указатель на команду
     */
    void push(std::shared_ptr<ICommand> command);

    /**
     * @brief Обработать одну команду из очереди
     * @return true, если команда была обработана, false если очередь пуста
     */
    bool processOne();

    /**
     * @brief Обработать все команды из очереди
     */
    void processAll();

    /**
     * @brief Проверить, пуста ли очередь
     * @return true, если очередь пуста, иначе false
     */
    bool isEmpty() const;

    /**
     * @brief Получить количество команд в очереди
     * @return Текущее количество команд
     */
    size_t size() const;

    /**
     * @brief Зарегистрировать обработчик исключений для конкретной команды
     * @tparam CommandType Тип команды
     * @tparam ExceptionType Тип исключения
     * @param handler Обработчик исключения
     */
    template<typename CommandType, typename ExceptionType>
    void registerHandler(std::shared_ptr<IExceptionHandler> handler) {
        HandlerKey key(
            std::type_index(typeid(CommandType)),
            std::type_index(typeid(ExceptionType))
        );
        handlers_[key] = handler;
    }

    /**
     * @brief Установить обработчик по умолчанию для всех команд
     * @param handler Обработчик исключений по умолчанию
     */
    void setDefaultHandler(std::shared_ptr<IExceptionHandler> handler);

private:
    /**
     * @brief Очередь команд
     */
    std::queue<std::shared_ptr<ICommand>> commands_;

    /**
     * @brief Карта зарегистрированных обработчиков
     */
    std::map<HandlerKey, std::shared_ptr<IExceptionHandler>> handlers_;

    /**
     * @brief Обработчик по умолчанию
     */
    std::shared_ptr<IExceptionHandler> defaultHandler_;

    /**
     * @brief Найти подходящий обработчик для конкретной команды и исключения
     * @param command Команда, в которой произошло исключение
     * @param exception Сгенерированное исключение
     * @return Умный указатель на обработчик или nullptr, если не найдено
     */
    std::shared_ptr<IExceptionHandler> findHandler(
        std::shared_ptr<ICommand> command,
        const std::exception& exception);
};
