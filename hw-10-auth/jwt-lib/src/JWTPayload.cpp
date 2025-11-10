#include "JWTPayload.hpp"
#include <ctime>

json JWTPayload::toJson() const {
    json j;
    j["sub"] = sub;
    j["iss"] = iss;
    j["iat"] = std::chrono::system_clock::to_time_t(iat);
    j["exp"] = std::chrono::system_clock::to_time_t(exp);
    j["gameId"] = gameId;
    j["userId"] = userId;
    j["role"] = role;
    return j;
}

JWTPayload JWTPayload::fromJson(const json& j) {
    JWTPayload payload;
    payload.sub = j.at("sub").get<std::string>();
    payload.iss = j.at("iss").get<std::string>();
    payload.iat = std::chrono::system_clock::from_time_t(j.at("iat").get<time_t>());
    payload.exp = std::chrono::system_clock::from_time_t(j.at("exp").get<time_t>());
    payload.gameId = j.at("gameId").get<std::string>();
    payload.userId = j.at("userId").get<std::string>();
    payload.role = j.value("role", "participant");
    return payload;
}

bool JWTPayload::isExpired() const {
    auto now = std::chrono::system_clock::now();
    return now > exp;
}