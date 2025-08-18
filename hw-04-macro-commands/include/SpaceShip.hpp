#pragma once

#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <unordered_map>

/**
 * @file SpaceShip.hpp
 * @brief Реализация космического корабля
 * @author Space Battle Team
 * @version 2.0
 */

 /**
  * @brief Реализация космического корабля
  *
  * Конкретная реализация игрового объекта - космического корабля.
  * Поддерживает все необходимые свойства для движения и поворота.
  * Использует целочисленные координаты и дискретную систему направлений.
  */
class SpaceShip : public IGameObject {
private:
    std::unordered_map<std::string, std::any> m_properties; ///< Карта свойств объекта

public:
	/// @brief Конструктор по умолчанию
    SpaceShip();

    /**
     * @brief Получить значение свойства
     * @param key Ключ свойства
     * @return Значение свойства
     * @throws std::runtime_error если свойство не найдено
     */
    std::any getProperty(const std::string& key) const override;

    /**
     * @brief Установить значение свойства
     * @param key Ключ свойства
     * @param value Новое значение
     * @throws std::runtime_error если свойство защищено от записи или значение некорректно
     */
    void setProperty(const std::string& key, const std::any& value) override;
};