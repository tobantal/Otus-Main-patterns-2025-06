#pragma once

#include "Vector2D.hpp"
#include <stdexcept>

/**
 * @file IVelocityChangeable.hpp
 * @brief Интерфейс для объектов с изменяемой скоростью
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс для объектов с изменяемой скоростью
  *
  * Обеспечивает абстракцию для объектов, у которых можно изменять
  * направление вектора скорости при сохранении модуля.
  */
class IVelocityChangeable {
public:
    virtual ~IVelocityChangeable() = default;

    /**
     * @brief Получить модуль скорости
     * @return Модуль скорости (скаляр)
     * @throws std::runtime_error если скорость невозможно прочитать
     */
    virtual int getVelocity() const = 0;

    /**
     * @brief Получить текущее направление
     * @return Направление в дискретных единицах
     * @throws std::runtime_error если направление невозможно прочитать
     */
    virtual int getDirection() const = 0;

    /**
     * @brief Получить общее количество возможных направлений
     * @return Количество дискретных направлений
     * @throws std::runtime_error если значение невозможно прочитать
     */
    virtual int getDirectionsNumber() const = 0;
};