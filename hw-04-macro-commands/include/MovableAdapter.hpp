#pragma once

#include "IMovable.hpp"
#include "IGameObject.hpp"
#include <memory>
#include <cmath>

/**
 * @file MovableAdapter.hpp
 * @brief Адаптер для преобразования IGameObject в IMovable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Адаптер для преобразования IGameObject в IMovable
  *
  * Реализует паттерн Adapter, позволяя использовать универсальные
  * игровые объекты через интерфейс IMovable.
  * Преобразует полярную систему координат (позиция, скорость, направление)
  * в декартову для операций движения.
  */
class MovableAdapter : public IMovable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< Адаптируемый игровой объект

public:
    /**
     * @brief Конструктор адаптера
     * @param gameObject Игровой объект для адаптации
     * @throws std::invalid_argument если gameObject равен nullptr
     */
    explicit MovableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief Получить текущую позицию объекта
     * @return Вектор позиции из свойства "Position"
     * @throws std::runtime_error если позицию невозможно прочитать
     */
    Vector2D getPosition() const override;

    /**
     * @brief Установить новую позицию объекта
     * @param position Новая позиция
     * @throws std::runtime_error если позицию невозможно установить
     */
    void setPosition(const Vector2D& position) override;

    /**
     * @brief Получить текущую скорость объекта в декартовых координатах
     *
     * Вычисляет скорость на основе полярных координат:
     * - velocity (модуль скорости)
     * - direction (направление)
     * - directionsNumber (количество дискретных направлений)
     *
     * @return Вектор скорости в декартовых координатах
     * @throws std::runtime_error если скорость невозможно вычислить
     */
    Vector2D getVelocity() const override;
};