#pragma once

#include <stdexcept>

/**
 * @file IRotatable.hpp
 * @brief Интерфейс для объектов, способных к повороту
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс для объектов, способных к повороту
  *
  * Обеспечивает абстракцию над объектами, которые могут поворачиваться
  * дискретными углами. Использует целочисленное представление направлений
  * для избежания ошибок округления при многократных поворотах.
  */
class IRotatable {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IRotatable() = default;

    /**
     * @brief Получить текущее направление объекта
     * @return Направление как целое число (0 до directionsNumber-1)
     * @throws std::runtime_error если направление невозможно прочитать
     */
    virtual int getDirection() const = 0;

    /**
     * @brief Установить новое направление объекта
     * @param direction Новое направление
     * @throws std::runtime_error если направление невозможно установить
     */
    virtual void setDirection(int direction) = 0;

    /**
     * @brief Получить угловую скорость объекта
     * @return Угловая скорость в дискретных единицах поворота за шаг
     * @throws std::runtime_error если угловую скорость невозможно прочитать
     */
    virtual int getAngularVelocity() const = 0;

    /**
     * @brief Получить общее количество возможных направлений
     * @return Количество дискретных направлений (например, 360 для градусов)
     */
    virtual int getDirectionsNumber() const = 0;
};