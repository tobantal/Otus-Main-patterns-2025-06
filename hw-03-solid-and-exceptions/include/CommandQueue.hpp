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
 * @brief Класс очереди команд с обработкой исключений
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Класс для управления очередью команд и их выполнением
  *
  * Реализует паттерн Command с возможностью обработки исключений
  * через различные стратегии (Strategy pattern)
  */
class CommandQueue {
public:
    /**
     * @brief Тип для хранения обработчиков исключений
     *
     * Ключ - пара (тип команды, тип исключения)
     */
    using HandlerKey = std::pair<std::type_index, std::type_index>;

    /**
     * @brief Конструктор по умолчанию
     */
    CommandQueue();

    /**
     * @brief Деструктор
     */
    ~CommandQueue() = default;

    /**
     * @brief Добавить команду в очередь
     * @param command Команда для добавления
     */
    void push(std::shared_ptr<ICommand> command);

    /**
     * @brief Обработать одну команду из очереди
     * @return true если команда была обработана, false если очередь пуста
     */
    bool processOne();

    /**
     * @brief Обработать все команды в очереди
     */
    void processAll();

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
     * @brief Зарегистрировать обработчик исключений
     * @tparam CommandType Тип команды
     * @tparam ExceptionType Тип исключения
     * @param handler Обработчик исключений
     */
    template<typename CommandType, typename ExceptionType>
    void registerHandler(std::shared_ptr<IExceptionHandler> handler) {
        HandlerKey key(std::type_index(typeid(CommandType)),
            std::type_index(typeid(ExceptionType)));
        handlers_[key] = handler;
    }

    /**
     * @brief Установить обработчик по умолчанию
     * @param handler Обработчик исключений по умолчанию
     */
    void setDefaultHandler(std::shared_ptr<IExceptionHandler> handler);

private:
    /**
     * @brief Очередь команд
     */
    std::queue<std::shared_ptr<ICommand>> commands_;

    /**
     * @brief Карта обработчиков исключений
     */
    std::map<HandlerKey, std::shared_ptr<IExceptionHandler>> handlers_;

    /**
     * @brief Обработчик исключений по умолчанию
     */
    std::shared_ptr<IExceptionHandler> defaultHandler_;

    /**
     * @brief Найти подходящий обработчик для исключения
     * @param command Команда, выбросившая исключение
     * @param exception Исключение
     * @return Указатель на обработчик или nullptr
     */
    std::shared_ptr<IExceptionHandler> findHandler(
        std::shared_ptr<ICommand> command,
        const std::exception& exception);
};