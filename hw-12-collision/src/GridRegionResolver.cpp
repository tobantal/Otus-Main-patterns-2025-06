#include "GridRegionResolver.hpp"
#include <stdexcept>
#include <any>

GridRegionResolver::GridRegionResolver(double cellSize, Vector2D offset)
    : cellSize_(cellSize), offset_(offset) 
{
    // Проверка валидности размера ячейки
    if (cellSize <= 0) {
        throw std::invalid_argument("Cell size must be positive");
    }
}

RegionId GridRegionResolver::getRegion(const IGameObject& object) const {
    // Получаем позицию объекта
    Vector2D pos = std::any_cast<Vector2D>(object.getProperty("Position"));
    
    // Применяем смещение сетки
    double adjustedX = pos.x - offset_.x;
    double adjustedY = pos.y - offset_.y;
    
    // Вычисляем индексы ячейки (floor для корректной работы с отрицательными)
    int regionX = static_cast<int>(std::floor(adjustedX / cellSize_));
    int regionY = static_cast<int>(std::floor(adjustedY / cellSize_));
    
    return RegionId(regionX, regionY);
}

double GridRegionResolver::getCellSize() const {
    return cellSize_;
}

Vector2D GridRegionResolver::getOffset() const {
    return offset_;
}