#pragma once
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/** 
 * @brief Структура полезной нагрузки JWT токена
 * Содержит стандартные поля и дополнительные поля для игры
 * @note Время в полях iat и exp хранится в виде std::chrono::system_clock::time_point
 * для удобства работы с временем в C++
 * 
 * @author Anton Tobolkin
 * 
 * @param sub Subject
 * @param iss Issuer
 * @param iat Issued At
 * @param exp Expiration Time
 * @param gameId Идентификатор игры
 * @param userId Идентификатор пользователя
 * @param role Роль пользователя в игре
 *
 */   
struct JWTPayload {
    std::string sub;
    std::string iss;
    std::chrono::system_clock::time_point iat;
    std::chrono::system_clock::time_point exp;
    std::string gameId;
    std::string userId;
    std::string role;
    
    json toJson() const;
    static JWTPayload fromJson(const json& j);
    bool isExpired() const;
};
