#pragma once

#include "IJWTGenerator.hpp"
#include <string>

/**
 * @brief Генератор JWT с RSA подписью (RS256)
 */
class RSAJWTGenerator : public IJWTGenerator {
public:
    /**
     * @brief Конструктор с загрузкой приватного ключа
     * 
     * @param privateKeyPath Путь к приватному ключу (PEM)
     * @throw JWTException если ключ не загружен
     */
    explicit RSAJWTGenerator(const std::string& privateKeyPath);

    ~RSAJWTGenerator() override = default;

    std::string generate(
        const JWTPayload& payload,
        std::chrono::seconds expirationTime = std::chrono::seconds(3600)
    ) override;

private:
    std::string privateKey_;

    void loadPrivateKey(const std::string& path);
};
