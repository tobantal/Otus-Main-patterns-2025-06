#include "OrderInterpreter.hpp"
#include "IoC.hpp"
#include <stdexcept>

/**
 * @file OrderInterpreter.cpp
 * @brief Реализация базового интерпретатора приказов
 * @author Anton Tobolkin
 */

void OrderInterpreter::interpret(const Order& order)
{
    // Шаг 1: Валидация приказа
    if (!order.isValid()) {
        throw std::runtime_error("Invalid order: missing id or action");
    }
    
    // Шаг 2: Разрешение объекта через IoC
    auto object = resolveObject(order.objectId);
    
    // Шаг 3: Создание команды через IoC
    auto command = createCommand(order.action, object, order.parameters);
    
    // Шаг 4: Выполнение команды
    command->execute();
}

std::shared_ptr<IUObject> OrderInterpreter::resolveObject(const std::string& objectId)
{
    try {
        // Формируем ключ для IoC: "Objects.{id}"
        std::string key = "Objects." + objectId;
        return IoC::resolve<IUObject>(key);
    } catch (const std::exception& e) {
        throw std::runtime_error("Object not found: " + objectId);
    }
}

std::shared_ptr<ICommand> OrderInterpreter::createCommand(
    const std::string& action,
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
{
    try {
        // Формируем ключ для IoC: "Commands.{action}"
        std::string key = "Commands." + action;
        
        // Формируем аргументы для фабрики команд
        // Фабрика ожидает: (object, params)
        std::vector<std::shared_ptr<void>> args = {
            std::static_pointer_cast<void>(object),
            std::static_pointer_cast<void>(params)
        };
        
        return IoC::resolve<ICommand>(key, args);
    } catch (const std::exception& e) {
        throw std::runtime_error("Unknown action: " + action);
    }
}