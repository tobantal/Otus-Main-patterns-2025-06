#include "GameRegistry.hpp"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>

std::string GameRegistry::generateGameId()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    ss << "game_";
    for (int i = 0; i < 16; ++i)
    {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string GameRegistry::createGame(
    const std::string &organizerId,
    const std::vector<std::string> &participants,
    std::chrono::seconds ttl)
{
    auto gameId = generateGameId();
    std::cerr << "[GameRegistry::createGame] Generated gameId: " << gameId << std::endl;

    auto now = std::chrono::system_clock::now();

    auto game = std::make_shared<GameInfo>();
    game->gameId = gameId;
    game->organizerId = organizerId;
    game->participants = participants;
    game->createdAt = now;
    game->expiresAt = now + ttl;

    games_.insert(gameId, game);

    return gameId;
}

std::shared_ptr<GameInfo> GameRegistry::getGame(const std::string &gameId) const
{
    return games_.find(gameId);
}

bool GameRegistry::isParticipant(const std::string &gameId, const std::string &userId) const
{
    auto game = games_.find(gameId);
    if (!game)
    {
        return false;
    }
    return game->isParticipant(userId);
}
