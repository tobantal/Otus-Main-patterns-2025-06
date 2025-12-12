#pragma once

#include <functional>

/**
 * @file RegionId.hpp
 * @brief Идентификатор региона (окрестности) на игровом поле
 * @author Anton Tobolkin
 */

/**
 * @brief Идентификатор региона в сетке
 * 
 * Представляет координаты ячейки в пространственной сетке.
 * Используется для группировки объектов по окрестностям.
 */
struct RegionId {
    int x;  ///< Координата X в сетке
    int y;  ///< Координата Y в сетке

    /**
     * @brief Конструктор по умолчанию (невалидный регион)
     */
    RegionId() : x(0), y(0) {}

    /**
     * @brief Конструктор с координатами
     */
    RegionId(int x, int y) : x(x), y(y) {}

    /**
     * @brief Оператор сравнения
     */
    bool operator==(const RegionId& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const RegionId& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Хэш-функция для RegionId
 * 
 * Позволяет использовать RegionId как ключ в unordered_map.
 */
struct RegionIdHash {
    std::size_t operator()(const RegionId& id) const {
        // Комбинируем хэши x и y
        return std::hash<int>()(id.x) ^ (std::hash<int>()(id.y) << 1);
    }
};