#include "commands/FireCommand.hpp"
#include <stdexcept>

/**
 * @file FireCommand.cpp
 * @brief Реализация команды выстрела
 * @author Anton Tobolkin
 */

FireCommand::FireCommand(
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
    : object_(object)
    , targetId_("")
{
    if (!object) {
        throw std::runtime_error("FireCommand: object cannot be null");
    }
    
    // targetId - опциональный параметр
    if (params) {
        try {
            targetId_ = std::any_cast<std::string>(params->getProperty("targetId"));
        } catch (...) {
            // Параметр отсутствует или неверного типа - используем пустую строку
            targetId_ = "";
        }
    }
}

void FireCommand::execute()
{
    // Записываем информацию о выстреле
    object_->setProperty("lastFireTarget", targetId_);
    
    // В реальной игре здесь была бы логика:
    // - создание снаряда
    // - уменьшение боеприпасов
    // - проверка перезарядки
    // и т.д.
}