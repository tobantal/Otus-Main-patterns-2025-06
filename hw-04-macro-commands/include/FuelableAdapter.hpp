#pragma once

#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file FuelableAdapter.hpp
 * @brief Адаптер для преобразования IGameObject в IFuelable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Адаптер для преобразования IGameObject в IFuelable
  *
  * Реализует паттерн Adapter, позволяя использовать универсальные
  * игровые объекты через интерфейс IFuelable.
  */
class FuelableAdapter : public IFuelable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< Адаптируемый игровой объект

public:
    /**
     * @brief Конструктор адаптера
     * @param gameObject Игровой объект для адаптации
     * @throws std::invalid_argument если gameObject равен nullptr
     */
    explicit FuelableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief Получить текущий уровень топлива
     * @return Уровень топлива из свойства "FuelLevel"
     * @throws std::runtime_error если уровень топлива невозможно прочитать
     */
    int getFuelLevel() const override;

    /**
     * @brief Установить новый уровень топлива
     * @param level Новый уровень топлива
     * @throws std::runtime_error если уровень топлива невозможно установить
     */
    void setFuelLevel(int level) override;

    /**
     * @brief Получить расход топлива за операцию
     * @return Расход топлива из свойства "FuelConsumption"
     * @throws std::runtime_error если расход топлива невозможно прочитать
     */
    int getFuelConsumption() const override;
};