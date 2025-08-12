#include "CommandQueue.hpp"
#include <iostream>

/**
 * @brief Конструктор по умолчанию
 */
CommandQueue::CommandQueue() : defaultHandler_(nullptr) {
}

/**
 * @brief Добавить команду в очередь
 */
void CommandQueue::push(std::shared_ptr<ICommand> command) {
    if (command) {
        commands_.push(command);
    }
}

/**
 * @brief Обработать одну команду из очереди
 */
bool CommandQueue::processOne() {
    if (commands_.empty()) {
        return false;
    }

	// Извлекаем первую команду из очереди
    auto command = commands_.front();
    // Удаляем команду из очереди
	commands_.pop();

    try {
        // Выполняем команду в блоке try-catch
        command->execute();
    }
    catch (const std::exception& e) {
        // Перехватываем только базовое исключение
        // Находим подходящий обработчик
        auto handler = findHandler(command, e);

        if (handler) {
            // Вызываем обработчик исключения
            handler->handle(command, e);
        }
        else if (defaultHandler_) {
            // Используем обработчик по умолчанию
            defaultHandler_->handle(command, e);
        }
        else {
            // Если обработчик не найден, перебрасываем исключение
            std::cerr << "No handler found for exception: " << e.what() << std::endl;
            throw;
        }
    }

    return true;
}

/**
 * @brief Обработать все команды в очереди
 */
void CommandQueue::processAll() {
    while (processOne()) {
        // Продолжаем обработку пока есть команды
    }
}

/**
 * @brief Проверить, пуста ли очередь
 */
bool CommandQueue::isEmpty() const {
    return commands_.empty();
}

/**
 * @brief Получить размер очереди
 */
size_t CommandQueue::size() const {
    return commands_.size();
}

/**
 * @brief Установить обработчик по умолчанию
 */
void CommandQueue::setDefaultHandler(std::shared_ptr<IExceptionHandler> handler) {
    defaultHandler_ = handler;
}

/**
 * @brief Найти подходящий обработчик для исключения
 */
std::shared_ptr<IExceptionHandler> CommandQueue::findHandler(
    std::shared_ptr<ICommand> command,
    const std::exception& exception) {

    // Создаем ключ для поиска обработчика
    HandlerKey key(std::type_index(typeid(*command)),
        std::type_index(typeid(exception)));

    // Ищем точное совпадение
    auto it = handlers_.find(key);
    if (it != handlers_.end()) {
        return it->second;
    }

    // Если точного совпадения нет, возвращаем nullptr
    return nullptr;
}