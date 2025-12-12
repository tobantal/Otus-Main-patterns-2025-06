#pragma once

#include "ICommand.hpp"
#include "ICollisionChecker.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file CheckCollisionCommand.hpp
 * @brief Команда проверки коллизии между двумя объектами
 * @author Anton Tobolkin
 */

/**
 * @brief Команда проверки коллизии пары объектов
 * 
 * Проверяет, столкнулись ли два игровых объекта.
 * Использует ICollisionChecker для выполнения проверки.
 */
class CheckCollisionCommand : public ICommand {
private:
    std::shared_ptr<IGameObject> object1_;
    std::shared_ptr<IGameObject> object2_;
    std::shared_ptr<ICollisionChecker> checker_;

public:
    /**
     * @brief Конструктор
     * @param object1 Первый объект
     * @param object2 Второй объект  
     * @param checker Проверщик коллизий
     */
    CheckCollisionCommand(std::shared_ptr<IGameObject> object1,
                          std::shared_ptr<IGameObject> object2,
                          std::shared_ptr<ICollisionChecker> checker);

    /**
     * @brief Выполнить проверку коллизии
     */
    void execute() override;

    /**
     * @brief Получить первый объект
     */
    std::shared_ptr<IGameObject> getObject1() const;

    /**
     * @brief Получить второй объект
     */
    std::shared_ptr<IGameObject> getObject2() const;
};