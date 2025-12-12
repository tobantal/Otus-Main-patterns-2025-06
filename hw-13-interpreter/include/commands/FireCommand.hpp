#pragma once

#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>
#include <string>

/**
 * @file FireCommand.hpp
 * @brief Команда выстрела игрового объекта
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда выстрела
 * 
 * Инициирует выстрел из объекта.
 * Опционально может принимать параметр "targetId" для прицельной стрельбы.
 * 
 * После выполнения объект получает свойство "lastFireTarget"
 * с ID цели (или пустую строку для стрельбы без цели).
 */
class FireCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * 
     * @param object Объект для управления
     * @param params Параметры команды (опционально targetId)
     */
    FireCommand(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);
    
    /**
     * @brief Выполняет команду выстрела
     * 
     * Устанавливает свойство "lastFireTarget" объекта.
     */
    void execute() override;

private:
    std::shared_ptr<IUObject> object_;  ///< Целевой объект
    std::string targetId_;               ///< ID цели (опционально)
};