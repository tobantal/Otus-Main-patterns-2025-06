#pragma once

#include "RegionId.hpp"
#include "IGameObject.hpp"

/**
 * @file IRegionResolver.hpp
 * @brief Интерфейс для определения региона по позиции объекта
 * @author Anton Tobolkin
 */

/**
 * @brief Интерфейс определения региона
 * 
 * Отвечает за вычисление идентификатора региона
 * на основе позиции игрового объекта.
 */
class IRegionResolver {
public:
    virtual ~IRegionResolver() = default;

    /**
     * @brief Определить регион для объекта
     * @param object Игровой объект с позицией
     * @return Идентификатор региона
     */
    virtual RegionId getRegion(const IGameObject& object) const = 0;

    /**
     * @brief Получить размер ячейки
     * @return Размер стороны ячейки
     */
    virtual double getCellSize() const = 0;
};