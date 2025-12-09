#pragma once

#include "RegionManager.hpp"
#include "GridRegionResolver.hpp"
#include "ICollisionChecker.hpp"
#include "MacroCommand.hpp"
#include <vector>
#include <memory>

/**
 * @file MultiRegionCollisionSystem.hpp
 * @brief Система проверки коллизий с несколькими смещёнными окрестностями
 * @author Anton Tobolkin
 */

/**
 * @brief Система с несколькими смещёнными системами окрестностей
 * 
 * Решает проблему границ: объекты, находящиеся на границе одной системы,
 * попадают в один регион в другой системе (со смещением).
 * 
 * Пример с 2 системами:
 * - Система 0: смещение (0, 0)
 * - Система 1: смещение (cellSize/2, cellSize/2)
 */
class MultiRegionCollisionSystem {
private:
    /// Менеджеры регионов для каждой системы
    std::vector<std::shared_ptr<RegionManager>> managers_;
    
    /// Проверщик коллизий
    std::shared_ptr<ICollisionChecker> collisionChecker_;
    
    /// Макрокоманды для каждой системы
    std::vector<std::shared_ptr<MacroCommand>> collisionMacros_;
    
    /// Размер ячейки
    double cellSize_;
    
    /// Количество систем
    int numSystems_;

public:
    /**
     * @brief Конструктор
     * @param cellSize Размер ячейки сетки
     * @param collisionChecker Проверщик коллизий
     * @param numSystems Количество систем окрестностей (по умолчанию 2)
     */
    MultiRegionCollisionSystem(double cellSize,
                               std::shared_ptr<ICollisionChecker> collisionChecker,
                               int numSystems = 2);

    /**
     * @brief Добавить объект во все системы
     * @param object Игровой объект
     */
    void addObject(std::shared_ptr<IGameObject> object);

    /**
     * @brief Удалить объект из всех систем
     * @param object Игровой объект
     */
    void removeObject(std::shared_ptr<IGameObject> object);

    /**
     * @brief Обновить позицию объекта во всех системах
     * @param object Игровой объект
     * 
     * Пересчитывает регион объекта во всех системах
     * и обновляет макрокоманды проверки коллизий.
     */
    void updateObject(std::shared_ptr<IGameObject> object);

    /**
     * @brief Получить все команды проверки коллизий
     * @return Вектор команд из всех систем
     */
    std::vector<std::shared_ptr<ICommand>> getCollisionCommands() const;

    /**
     * @brief Получить количество систем окрестностей
     */
    int getNumSystems() const;

    /**
     * @brief Получить менеджер для указанной системы
     * @param index Индекс системы (0..numSystems-1)
     */
    std::shared_ptr<RegionManager> getManager(int index) const;
};