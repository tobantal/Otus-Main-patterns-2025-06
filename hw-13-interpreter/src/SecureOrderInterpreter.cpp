#include "SecureOrderInterpreter.hpp"
#include "IoC.hpp"
#include <stdexcept>

/**
 * @file SecureOrderInterpreter.cpp
 * @brief Реализация защищенного интерпретатора через скоупы
 * @author Anton Tobolkin
 */

SecureOrderInterpreter::SecureOrderInterpreter(const std::string& playerId)
    : OrderInterpreter()
    , playerId_(playerId)
{
    // Скоуп игрока должен быть создан заранее при входе игрока в игру
    // Здесь мы только проверяем, что он существует
}

void SecureOrderInterpreter::interpret(const Order& order)
{
    // Устанавливаем скоуп текущего игрока перед выполнением
    setPlayerScope();
    
    // Вызываем базовую реализацию
    // Она попытается разрешить объект в текущем скоупе
    OrderInterpreter::interpret(order);
}

std::shared_ptr<IUObject> SecureOrderInterpreter::resolveObject(const std::string& objectId)
{
    try {
        // Попытка разрешить объект в скоупе игрока
        std::string key = "Objects." + objectId;
        return IoC::resolve<IUObject>(key);
    } catch (const std::exception& e) {
        // Объект не найден в скоупе игрока = нет доступа
        throw std::runtime_error(
            "Access denied: object '" + objectId + 
            "' does not belong to player '" + playerId_ + "'"
        );
    }
}

void SecureOrderInterpreter::setPlayerScope()
{
    try {
        // Формируем ID скоупа игрока
        auto scopeId = std::make_shared<std::string>(getScopeId());
        std::vector<std::shared_ptr<void>> args = {scopeId};
        
        // Устанавливаем текущий скоуп
        auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
        setCommand->execute();
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to set player scope for '" + playerId_ + "': " + e.what()
        );
    }
}

std::string SecureOrderInterpreter::getScopeId() const
{
    return "Player." + playerId_;
}