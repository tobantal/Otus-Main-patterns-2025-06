#pragma once

#include "Order.hpp"

/**
 * @file IOrderInterpreter.hpp
 * @brief Интерфейс интерпретатора приказов
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерфейс интерпретатора приказов
 */
class IOrderInterpreter {
public:
    virtual ~IOrderInterpreter() = default;
    
    /**
     * @brief Интерпретирует и выполняет приказ
     * @param order Доменная модель приказа
     * @throws std::runtime_error при ошибке интерпретации
     */
    virtual void interpret(const Order& order) = 0;
};