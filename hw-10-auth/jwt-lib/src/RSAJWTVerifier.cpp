#include "RSAJWTVerifier.hpp"
#include "JWTException.hpp"
#include <fstream>
#include <sstream>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

using json = nlohmann::json;

RSAJWTVerifier::RSAJWTVerifier(const std::string &publicKeyPath)
{
    loadPublicKey(publicKeyPath);
}

void RSAJWTVerifier::loadPublicKey(const std::string &path)
{
    std::ifstream keyFile(path);
    if (!keyFile.is_open())
    {
        throw JWTException("Cannot open public key file: " + path);
    }

    std::stringstream buffer;
    buffer << keyFile.rdbuf();
    publicKey_ = buffer.str();

    if (publicKey_.empty())
    {
        throw JWTException("Public key file is empty: " + path);
    }
}

JWTPayload RSAJWTVerifier::verify(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);

        // Верифицируем подпись
        auto verifier = jwt::verify<jwt::traits::nlohmann_json>()
            .allow_algorithm(jwt::algorithm::rs256(publicKey_));
        
        std::error_code ec;
        verifier.verify(decoded, ec);
        
        if (ec) {
            throw JWTException("Invalid signature: " + ec.message());
        }

        // Получаем payload и парсим
        auto payload_str = decoded.get_payload();
        auto payload_json = json::parse(payload_str);

        // Проверяем exp
        auto exp_claim = decoded.get_payload_claim("exp");
        auto exp_date = exp_claim.as_date();
        auto now = std::chrono::system_clock::now();

        if (now > exp_date)
        {
            throw JWTException("Token has expired");
        }

        return JWTPayload::fromJson(payload_json);
    }
    catch (const JWTException &)
    {
        throw;
    }
    catch (const std::exception &e)
    {
        throw JWTException(std::string("Failed to verify JWT: ") + e.what());
    }
}

bool RSAJWTVerifier::isValid(const std::string &token) noexcept
{
    try
    {
        verify(token);
        return true;
    }
    catch (...)
    {
        return false;
    }
}
