#pragma once

#include "IGameObject.hpp"
#include <memory>

/**
 * @file ICollisionChecker.hpp
 * @brief Интерфейс для проверки коллизий между объектами
 * @author Anton Tobolkin
 * 
 * @note По условию задания реализация не требуется.
 *       Это заглушка для демонстрации архитектуры.
 */

/**
 * @brief Интерфейс проверки коллизий
 * 
 * Определяет, столкнулись ли два игровых объекта.
 */
class ICollisionChecker {
public:
    virtual ~ICollisionChecker() = default;

    /**
     * @brief Проверить коллизию между двумя объектами
     * @param obj1 Первый объект
     * @param obj2 Второй объект
     * @return true если объекты столкнулись
     */
    virtual bool checkCollision(const IGameObject& obj1, 
                                const IGameObject& obj2) const = 0;
};