#pragma once

#include "ICommand.hpp"
#include "IRegionManager.hpp"
#include "ICollisionChecker.hpp"
#include "MacroCommand.hpp"
#include <memory>
#include <string>

/**
 * @file UpdateRegionCommand.hpp
 * @brief Команда обновления региона игрового объекта
 * @author Anton Tobolkin
 */

/**
 * @brief Команда обновления региона и создания команд проверки коллизий
 * 
 * Выполняет:
 * 1. Определение текущей окрестности объекта
 * 2. Перемещение объекта между окрестностями при необходимости
 * 3. Создание команд проверки коллизий с соседями
 * 4. Обновление макрокоманды коллизий
 */
class UpdateRegionCommand : public ICommand {
private:
    std::shared_ptr<IGameObject> object_;
    std::shared_ptr<IRegionManager> regionManager_;
    std::shared_ptr<ICollisionChecker> collisionChecker_;
    
    /// Ссылка на макрокоманду для обновления (внешняя)
    std::shared_ptr<MacroCommand>& collisionMacro_;
    
    /// Ключ свойства для хранения текущего региона
    static const std::string REGION_KEY;

public:
    /**
     * @brief Конструктор
     * @param object Игровой объект для отслеживания
     * @param regionManager Менеджер регионов
     * @param collisionChecker Проверщик коллизий
     * @param collisionMacro Ссылка на макрокоманду (будет обновляться)
     */
    UpdateRegionCommand(std::shared_ptr<IGameObject> object,
                        std::shared_ptr<IRegionManager> regionManager,
                        std::shared_ptr<ICollisionChecker> collisionChecker,
                        std::shared_ptr<MacroCommand>& collisionMacro);

    /**
     * @brief Выполнить обновление региона
     */
    void execute() override;

private:
    /**
     * @brief Получить сохранённый регион объекта
     * @return Регион или невалидный RegionId если не сохранён
     */
    RegionId getStoredRegion() const;

    /**
     * @brief Сохранить регион в свойствах объекта
     */
    void storeRegion(const RegionId& region);

    /**
     * @brief Проверить, был ли объект уже добавлен в систему
     */
    bool hasStoredRegion() const;
};