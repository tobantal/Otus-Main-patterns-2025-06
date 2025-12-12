#pragma once

#include "IUObject.hpp"
#include <string>
#include <memory>

/**
 * @file Order.hpp
 * @brief Доменная модель приказа для игровых объектов
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Доменная модель приказа
 * 
 * Не зависит от формата сериализации (JSON, XML, Protobuf и т.д.).
 * Параметры хранятся как IUObject для унифицированного доступа.
 * 
 * Приказ содержит:
 * - ID объекта-получателя
 * - Название действия для выполнения
 * - Параметры действия в виде универсального объекта
 */
struct Order {
    std::string objectId;                      ///< ID объекта-получателя приказа
    std::string action;                        ///< Действие для выполнения (StartMove, Fire и т.д.)
    std::shared_ptr<IUObject> parameters;      ///< Параметры действия
    
    /**
     * @brief Проверяет валидность приказа
     * 
     * Приказ считается валидным, если заполнены обязательные поля:
     * objectId и action.
     * 
     * @return true если приказ содержит обязательные поля
     */
    bool isValid() const {
        return !objectId.empty() && !action.empty();
    }
};