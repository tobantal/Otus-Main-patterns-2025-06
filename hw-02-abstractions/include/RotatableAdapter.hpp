#pragma once

#include "IRotatable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file RotatableAdapter.hpp
 * @brief Адаптер для преобразования IGameObject в IRotatable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Адаптер для преобразования IGameObject в IRotatable
  *
  * Реализует паттерн Adapter, позволяя использовать универсальные
  * игровые объекты через интерфейс IRotatable.
  */
class RotatableAdapter : public IRotatable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< Адаптируемый игровой объект

public:
    /**
     * @brief Конструктор адаптера
     * @param gameObject Игровой объект для адаптации
     * @throws std::invalid_argument если gameObject равен nullptr
     */
    explicit RotatableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief Получить текущее направление объекта
     * @return Направление из свойства "Direction"
     * @throws std::runtime_error если направление невозможно прочитать
     */
    int getDirection() const override;

    /**
     * @brief Установить новое направление объекта
     * @param direction Новое направление
     * @throws std::runtime_error если направление невозможно установить
     */
    void setDirection(int direction) override;

    /**
     * @brief Получить угловую скорость объекта
     * @return Угловая скорость из свойства "AngularVelocity"
     * @throws std::runtime_error если угловую скорость невозможно прочитать
     */
    int getAngularVelocity() const override;

    /**
     * @brief Получить общее количество возможных направлений
     * @return Количество направлений из свойства "DirectionsNumber"
     * @throws std::runtime_error если значение невозможно прочитать
     */
    int getDirectionsNumber() const override;
};