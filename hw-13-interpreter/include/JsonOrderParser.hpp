#pragma once

#include "IOrderParser.hpp"

/**
 * @file JsonOrderParser.hpp
 * @brief Парсер приказов из JSON формата
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Парсер приказов из JSON формата
 * 
 * Единственное место в системе, знающее о JSON.
 * Преобразует JSON-строку в доменную модель Order.
 * 
 * Ожидаемый формат JSON:
 * @code
 * {
 *   "id": "object_id",
 *   "action": "ActionName",
 *   "param1": value1,
 *   "param2": value2,
 *   ...
 * }
 * @endcode
 * 
 * Поля "id" и "action" являются обязательными.
 * Остальные поля трактуются как параметры команды.
 */
class JsonOrderParser : public IOrderParser {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    JsonOrderParser() = default;
    
    /**
     * @brief Виртуальный деструктор
     */
    ~JsonOrderParser() override = default;
    
    /**
     * @brief Парсит JSON-строку в Order
     * 
     * @param jsonString JSON-строка с приказом
     * @return Order Распарсенный приказ
     * @throws std::runtime_error при ошибке парсинга JSON
     * @throws std::runtime_error если отсутствует поле "id"
     * @throws std::runtime_error если отсутствует поле "action"
     */
    Order parse(const std::string& jsonString) override;
};