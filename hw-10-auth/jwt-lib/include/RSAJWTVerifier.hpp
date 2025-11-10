#pragma once

#include "IJWTVerifier.hpp"
#include <string>

/**
 * @brief Проверка JWT с RSA подписью (RS256)
 */
class RSAJWTVerifier : public IJWTVerifier {
public:
    /**
     * @brief Конструктор с загрузкой публичного ключа
     * 
     * @param publicKeyPath Путь к публичному ключу (PEM)
     * @throw JWTException если ключ не загружен
     */
    explicit RSAJWTVerifier(const std::string& publicKeyPath);

    ~RSAJWTVerifier() override = default;

    JWTPayload verify(const std::string& token) override;

    bool isValid(const std::string& token) noexcept override;

private:
    std::string publicKey_;

    void loadPublicKey(const std::string& path);
};
