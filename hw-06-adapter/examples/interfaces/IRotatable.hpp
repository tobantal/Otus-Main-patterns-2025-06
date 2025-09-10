/**
 * @file IRotatable.hpp
 * @brief Интерфейс для поворачиваемых объектов
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

/**
 * @brief Интерфейс для объектов, способных к вращению
 * 
 * Определяет контракт для объектов, которые могут изменять
 * свое направление в дискретной системе направлений.
 */
class IRotatable {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IRotatable() = default;

    /**
     * @brief Получить текущее направление объекта
     * @return Направление из свойства "Direction"
     */
    virtual int getDirection() const = 0;

    /**
     * @brief Установить новое направление объекта  
     * @param direction Новое направление
     */
    virtual void setDirection(int direction) = 0;

    /**
     * @brief Получить угловую скорость объекта
     * @return Угловая скорость из свойства "AngularVelocity"
     */
    virtual int getAngularVelocity() const = 0;

    /**
     * @brief Получить общее количество возможных направлений
     * @return Количество направлений из свойства "DirectionsNumber"
     */
    virtual int getDirectionsNumber() const = 0;
};