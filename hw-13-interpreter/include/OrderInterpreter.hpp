#pragma once

#include "IOrderInterpreter.hpp"
#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>
#include <string>

/**
 * @file OrderInterpreter.hpp
 * @brief Базовая реализация интерпретатора приказов
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерпретатор приказов для игровых объектов
 * 
 * Использует IoC контейнер для динамического создания команд,
 * что позволяет добавлять новые команды без изменения кода
 * интерпретатора.
 * 
 * Схема работы:
 * 1. Валидация приказа (проверка обязательных полей)
 * 2. Разрешение объекта через IoC: "Objects.{id}"
 * 3. Создание команды через IoC: "Commands.{action}"
 * 4. Выполнение команды
 * 
 * Для расширения функциональности (добавления новых команд)
 * достаточно зарегистрировать фабрику в IoC с ключом
 * "Commands.{НазваниеКоманды}".
 */
class OrderInterpreter : public IOrderInterpreter {
public:
    /**
     * @brief Конструктор по умолчанию
     * 
     * Использует глобальный IoC контейнер
     */
    OrderInterpreter() = default;
    
    /**
     * @brief Виртуальный деструктор
     */
    ~OrderInterpreter() override = default;
    
    /**
     * @brief Интерпретирует и выполняет приказ
     * 
     * @param order Приказ для выполнения
     * @throws std::runtime_error если приказ невалиден
     * @throws std::runtime_error если объект не найден
     * @throws std::runtime_error если действие неизвестно
     */
    void interpret(const Order& order) override;

protected:
    /**
     * @brief Разрешает объект по ID через IoC
     * 
     * Ищет объект по ключу "Objects.{objectId}" в текущем скоупе.
     * 
     * @param objectId Идентификатор объекта
     * @return Указатель на IUObject
     * @throws std::runtime_error если объект не найден
     */
    virtual std::shared_ptr<IUObject> resolveObject(const std::string& objectId);
    
    /**
     * @brief Создает команду для действия через IoC
     * 
     * Ищет фабрику команды по ключу "Commands.{action}".
     * Передает объект и параметры в фабрику.
     * 
     * @param action Название действия
     * @param object Целевой объект
     * @param params Параметры команды
     * @return Указатель на созданную команду
     * @throws std::runtime_error если действие неизвестно
     */
    virtual std::shared_ptr<ICommand> createCommand(
        const std::string& action,
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);
};