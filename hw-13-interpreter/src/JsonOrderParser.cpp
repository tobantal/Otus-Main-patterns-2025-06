#include "JsonOrderParser.hpp"
#include "UObject.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>

/**
 * @file JsonOrderParser.cpp
 * @brief Реализация парсера приказов из JSON
 * @author Anton Tobolkin
 */

Order JsonOrderParser::parse(const std::string& jsonString)
{
    // Парсим JSON
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(jsonString);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }
    
    // Валидация обязательных полей
    if (!json.contains("id")) {
        throw std::runtime_error("Order must contain 'id' field");
    }
    if (!json.contains("action")) {
        throw std::runtime_error("Order must contain 'action' field");
    }
    
    // Создаем Order
    Order order;
    order.objectId = json["id"].get<std::string>();
    order.action = json["action"].get<std::string>();
    
    // Конвертируем параметры в UObject
    auto params = std::make_shared<UObject>();
    
    // Копируем все поля кроме id и action как параметры
    for (auto& [key, value] : json.items()) {
        if (key == "id" || key == "action") {
            continue;
        }
        
        // Определяем тип значения и сохраняем
        if (value.is_number_integer()) {
            params->setProperty(key, value.get<int>());
        } else if (value.is_number_float()) {
            params->setProperty(key, value.get<double>());
        } else if (value.is_string()) {
            params->setProperty(key, value.get<std::string>());
        } else if (value.is_boolean()) {
            params->setProperty(key, value.get<bool>());
        }
        // Сложные типы (массивы, вложенные объекты) игнорируем для простоты
        // При необходимости можно добавить поддержку
    }
    
    order.parameters = params;
    
    return order;
}