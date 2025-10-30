#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Исключение для JWT операций
 */
class JWTException : public std::runtime_error {
public:
    explicit JWTException(const std::string& message)
        : std::runtime_error(message) {}
};