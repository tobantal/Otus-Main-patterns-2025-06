#pragma once

#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <unordered_map>

/**
 * @file PhotonTorpedo.hpp
 * @brief Реализация фотонной торпеды
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Реализация фотонной торпеды
  *
  * Конкретная реализация игрового объекта - фотонной торпеды.
  * Поддерживает только прямолинейное движение без поворота.
  */
class PhotonTorpedo : public IGameObject {
private:
    std::unordered_map<std::string, std::any> m_properties; ///< Карта свойств объекта

public:
    /**
     * @brief Конструктор фотонной торпеды
     * @param position Начальная позиция
     * @param velocity Скорость в декартовых координатах
     */
    PhotonTorpedo(const Vector2D& position = Vector2D(0, 0),
        const Vector2D& velocity = Vector2D(0, 0));

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
     * @throws std::runtime_error если попытка записи в недоступное свойство
     */
    void setProperty(const std::string& key, const std::any& value) override;
};