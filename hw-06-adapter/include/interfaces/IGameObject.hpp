#pragma once

#include <any>
#include <string>
#include <stdexcept>

/**
 * @file IGameObject.hpp
 * @brief абстрактный игровой объект
 * @author Anton Tobolkin
 * @version 1.1
 */
class IGameObject {
public:

    virtual ~IGameObject() = default;

    /**
     * @brief получить параметр
     * @param key ключ
     * @return значение параметра
     * @throws std::runtime_error ошибка
     */
    virtual std::any getProperty(const std::string& key) const = 0;

    /**
     * @brief задать параметр
     * @param key ключ
     * @param value новое значение
     * @throws std::runtime_error ошибка
     */
    virtual void setProperty(const std::string& key, const std::any& value) = 0;
};