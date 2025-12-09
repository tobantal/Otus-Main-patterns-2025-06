#pragma once

#include "IRegionResolver.hpp"
#include "Vector2D.hpp"
#include <cmath>

/**
 * @file GridRegionResolver.hpp
 * @brief Реализация определения региона по регулярной сетке
 * @author Anton Tobolkin
 */

/**
 * @brief Определение региона по регулярной сетке
 * 
 * Разбивает пространство на квадратные ячейки заданного размера.
 * Поддерживает смещение сетки для создания перекрывающихся систем.
 */
class GridRegionResolver : public IRegionResolver {
private:
    double cellSize_;   ///< Размер стороны ячейки
    Vector2D offset_;   ///< Смещение сетки

public:
    /**
     * @brief Конструктор
     * @param cellSize Размер стороны ячейки (должен быть > 0)
     * @param offset Смещение сетки (по умолчанию {0, 0})
     */
    GridRegionResolver(double cellSize, Vector2D offset = Vector2D(0, 0));

    /**
     * @brief Определить регион для объекта
     * @param object Игровой объект (должен иметь свойство "Position")
     * @return Идентификатор региона
     */
    RegionId getRegion(const IGameObject& object) const override;

    /**
     * @brief Получить размер ячейки
     */
    double getCellSize() const override;

    /**
     * @brief Получить смещение сетки
     */
    Vector2D getOffset() const;
};