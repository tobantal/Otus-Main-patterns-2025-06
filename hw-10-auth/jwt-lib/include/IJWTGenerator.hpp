#pragma once

#include <string>
#include <chrono>
#include "JWTPayload.hpp"

/**
 * @brief Интерфейс для генерации JWT токенов
 * 
 * Паттерн: Strategy
 */
class IJWTGenerator {
public:
    virtual ~IJWTGenerator() = default;

    /**
     * @brief Генерирует JWT токен
     * 
     * @param payload Данные для включения в токен
     * @param expirationTime Время истечения от текущего момента
     * @return JWT токен (header.payload.signature)
     * @throw JWTException если ошибка генерации
     */
    virtual std::string generate(
        const JWTPayload& payload,
        std::chrono::seconds expirationTime = std::chrono::seconds(3600)
    ) = 0;
};
