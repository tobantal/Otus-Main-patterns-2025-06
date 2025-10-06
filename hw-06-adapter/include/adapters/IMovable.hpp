/**
 * @file IMovable.hpp
 * @brief Интерфейс для подвижных объектов
 * @author Anton Tobolkin  
 * @version 1.0
 */

#pragma once

#include "Vector2D.hpp"

/**
 * @brief Интерфейс для объектов, способных к перемещению
 * 
 * Определяет контракт для всех объектов, которые могут изменять
 * свое положение в пространстве.
 */
class IMovable {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IMovable() = default;

    /**
     * @brief Получить текущую позицию объекта
     * @return Текущие координаты объекта
     */
    virtual Vector2D getPosition() const = 0;

    /**
     * @brief Установить новую позицию объекта
     * @param position Новые координаты
     */
    virtual void setPosition(const Vector2D& position) = 0;

    /**
     * @brief Получить вектор скорости объекта
     * TODO: указывать ключ в IoC, т.е. IMovable:Velocity.get
     * или вообще убрать @custom_impl с метода.
     * 
     * @custom_impl MovableImplementations::getVelocity
     * @return Вектор скорости с учетом направления и модуля скорости
     */
    virtual Vector2D getVelocity() const = 0;
};