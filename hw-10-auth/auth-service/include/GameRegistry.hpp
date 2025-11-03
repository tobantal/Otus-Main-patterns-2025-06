#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include <ThreadSafeMap.hpp>

/**
 * @brief Информация об игре
 */
struct GameInfo
{
    std::string gameId;
    std::string organizerId;
    std::vector<std::string> participants;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point expiresAt;

    bool isParticipant(const std::string &userId) const
    {
        for (const auto &p : participants)
        {
            if (p == userId)
                return true;
        }
        return false;
    }
};

/**
 * @brief Thread-safe реестр игр
 */
class GameRegistry
{
public:
    // Запретить копирование!
    GameRegistry(const GameRegistry&) = delete;
    GameRegistry& operator=(const GameRegistry&) = delete;
    
    // Только move разрешен
    GameRegistry(GameRegistry&&) = default;
    GameRegistry& operator=(GameRegistry&&) = default;
    
    GameRegistry() = default;

    std::string createGame(
        const std::string &organizerId,
        const std::vector<std::string> &participants,
        std::chrono::seconds ttl);

    std::shared_ptr<GameInfo> getGame(const std::string &gameId) const;

    bool isParticipant(const std::string &gameId, const std::string &userId) const;

private:
    std::string generateGameId();

    ThreadSafeMap<std::string, GameInfo> games_;
};
