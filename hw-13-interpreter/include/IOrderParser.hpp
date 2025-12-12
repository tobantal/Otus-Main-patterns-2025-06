#pragma once

#include "Order.hpp"
#include <string>

/**
 * @file IOrderParser.hpp
 * @brief Интерфейс парсера приказов
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерфейс парсера приказов
 * 
 * Абстрагирует формат входных данных от доменной модели.
 * Позволяет поддерживать разные форматы сериализации:
 * JSON, XML, Protobuf, бинарный и т.д.
 * 
 * Каждая реализация отвечает за преобразование конкретного
 * формата в универсальную доменную модель Order.
 */
class IOrderParser {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IOrderParser() = default;
    
    /**
     * @brief Парсит входные данные в Order
     * 
     * @param input Строка с данными приказа в соответствующем формате
     * @return Order Распарсенный приказ
     * @throws std::runtime_error при ошибке парсинга или невалидных данных
     */
    virtual Order parse(const std::string& input) = 0;
};