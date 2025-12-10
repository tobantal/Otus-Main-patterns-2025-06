#include "OrderInterpreter.hpp"
#include "ICommandFactory.hpp"
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
    
    // Шаг 3: Создание команды через IoC (используя фабрику)
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
        
        // Получаем фабрику команды из IoC
        auto factory = IoC::resolve<ICommandFactory>(key);
        
        // Создаем команду через фабрику
        return factory->create(object, params);
    } catch (const std::exception& e) {
        throw std::runtime_error("Unknown action: " + action);
    }
}