#pragma once

#include <httplib.h>
#include <memory>
#include <IJWTVerifier.hpp>
#include <game_manager.hpp>

class BattleController : public std::enable_shared_from_this<BattleController>
{
private:
    std::shared_ptr<GameManager> gameManager_;
    std::shared_ptr<IJWTVerifier> jwtVerifier_;

public:
    BattleController(
        std::shared_ptr<GameManager> gameManager,
        std::shared_ptr<IJWTVerifier> jwtVerifier);

    void registerRoutes(httplib::Server &server);

private:
    void handleGameCommand(const httplib::Request &req, httplib::Response &res);
};
