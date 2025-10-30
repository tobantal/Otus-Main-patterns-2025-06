#include "RSAJWTGenerator.hpp"
#include "JWTException.hpp"
#include <fstream>
#include <sstream>
#include <chrono>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

using json = nlohmann::json;

RSAJWTGenerator::RSAJWTGenerator(const std::string &privateKeyPath)
{
    loadPrivateKey(privateKeyPath);
}

void RSAJWTGenerator::loadPrivateKey(const std::string &path)
{
    std::ifstream keyFile(path);
    if (!keyFile.is_open())
    {
        throw JWTException("Cannot open private key file: " + path);
    }

    std::stringstream buffer;
    buffer << keyFile.rdbuf();
    privateKey_ = buffer.str();

    if (privateKey_.empty())
    {
        throw JWTException("Private key file is empty: " + path);
    }
}

std::string RSAJWTGenerator::generate(
    const JWTPayload &payload,
    std::chrono::seconds expirationTime)
{

    try
    {
        // Вычисляем время истечения
        auto now = std::chrono::system_clock::now();
        auto exp = now + expirationTime;

        // Создаём payload с временем
        JWTPayload finalPayload = payload;
        finalPayload.iat = now;
        finalPayload.exp = exp;
        finalPayload.iss = "auth-service";
        finalPayload.sub = payload.userId;

        // Преобразуем в JSON
        json payloadJson = finalPayload.toJson();

        // Создаём JWT с RSA подписью
        auto builder = jwt::create<jwt::traits::nlohmann_json>()
                           .set_header_claim("alg", "RS256")
                           .set_header_claim("typ", "JWT");

        for (auto &[key, value] : payloadJson.items())
        {
            builder.set_payload_claim(key, value);
        }

        auto token = builder.sign(jwt::algorithm::rs256(
            "",
            privateKey_,
            "",
            ""));

        return token;
    }
    catch (const JWTException &)
    {
        throw;
    }
    catch (const std::exception &e)
    {
        throw JWTException(std::string("Failed to generate JWT: ") + e.what());
    }
}
