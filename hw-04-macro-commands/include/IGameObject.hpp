#pragma once

#include <any>
#include <string>
#include <stdexcept>

/**
 * @file IGameObject.hpp
 * @brief Универсальный интерфейс игрового объекта
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Универсальный интерфейс игрового объекта
  *
  * Предоставляет общий способ доступа к свойствам любого игрового объекта
  * через паттерн "Свойство". Это обеспечивает расширяемость системы -
  * можно добавлять новые типы объектов без изменения существующего кода.
  */
class IGameObject {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IGameObject() = default;

    /**
     * @brief Получить значение свойства объекта
     * @param key Ключ свойства
     * @return Значение свойства
     * @throws std::runtime_error если свойство не найдено или недоступно
     */
    virtual std::any getProperty(const std::string& key) const = 0;

    /**
     * @brief Установить значение свойства объекта
     * @param key Ключ свойства
     * @param value Новое значение
     * @throws std::runtime_error если свойство нельзя установить
     */
    virtual void setProperty(const std::string& key, const std::any& value) = 0;
};