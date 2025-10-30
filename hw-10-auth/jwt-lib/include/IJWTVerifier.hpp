#pragma once

#include <string>
#include "JWTPayload.hpp"

/**
 * @brief Интерфейс для проверки JWT токенов
 * 
 * Паттерн: Strategy
 */
class IJWTVerifier {
public:
    virtual ~IJWTVerifier() = default;

    /**
     * @brief Проверяет JWT токен и возвращает payload
     * 
     * @param token JWT токен
     * @return Payload из токена
     * @throw JWTException если токен невалидный или истёк
     */
    virtual JWTPayload verify(const std::string& token) = 0;

    /**
     * @brief Быстрая проверка без исключений
     * 
     * @param token JWT токен
     * @return true если валидный и не истёк
     */
    virtual bool isValid(const std::string& token) noexcept = 0;
};
