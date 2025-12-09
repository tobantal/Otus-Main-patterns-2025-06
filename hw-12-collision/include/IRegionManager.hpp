#pragma once

#include "RegionId.hpp"
#include "IRegionResolver.hpp"
#include "IGameObject.hpp"
#include <vector>
#include <memory>

/**
 * @file IRegionManager.hpp
 * @brief Интерфейс менеджера регионов для группировки объектов
 * @author Anton Tobolkin
 */

/**
 * @brief Интерфейс менеджера регионов
 * 
 * Управляет распределением игровых объектов по регионам.
 * Позволяет добавлять, удалять и запрашивать объекты в регионах.
 */
class IRegionManager {
public:
    virtual ~IRegionManager() = default;

    /**
     * @brief Добавить объект в соответствующий регион
     * @param object Игровой объект
     */
    virtual void addObject(std::shared_ptr<IGameObject> object) = 0;

    /**
     * @brief Удалить объект из его текущего региона
     * @param object Игровой объект
     */
    virtual void removeObject(std::shared_ptr<IGameObject> object) = 0;

    /**
     * @brief Переместить объект между регионами
     * @param object Игровой объект
     * @param oldRegion Старый регион
     * @param newRegion Новый регион
     */
    virtual void moveObject(std::shared_ptr<IGameObject> object, 
                           const RegionId& oldRegion, 
                           const RegionId& newRegion) = 0;

    /**
     * @brief Получить все объекты в указанном регионе
     * @param region Идентификатор региона
     * @return Вектор указателей на объекты
     */
    virtual std::vector<std::shared_ptr<IGameObject>> 
        getObjectsInRegion(const RegionId& region) const = 0;

    /**
     * @brief Получить resolver для определения регионов
     * @return Ссылка на resolver
     */
    virtual const IRegionResolver& getResolver() const = 0;
};