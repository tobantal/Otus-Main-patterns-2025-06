#pragma once

#include "Vector2D.hpp"
#include <stdexcept>

/**
 * @file IMovable.hpp
 * @brief Интерфейс для объектов, способных к прямолинейному движению
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс для объектов, способных к прямолинейному движению
  *
  * Данный интерфейс обеспечивает абстракцию над объектами, которые могут
  * перемещаться в двумерном пространстве с постоянной скоростью.
  * Следует принципу Interface Segregation - содержит только методы,
  * необходимые для движения.
  */
class IMovable {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IMovable() = default;

    /**
     * @brief Получить текущую позицию объекта
     * @return Вектор текущей позиции
     * @throws std::runtime_error если позицию невозможно прочитать
     */
    virtual Vector2D getPosition() const = 0;

    /**
     * @brief Установить новую позицию объекта
     * @param position Новая позиция
     * @throws std::runtime_error если позицию невозможно установить
     */
    virtual void setPosition(const Vector2D& position) = 0;

    /**
     * @brief Получить текущую скорость объекта
     * @return Вектор скорости
     * @throws std::runtime_error если скорость невозможно прочитать
     */
    virtual Vector2D getVelocity() const = 0;
};