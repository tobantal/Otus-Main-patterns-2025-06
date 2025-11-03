#include "BattleController.hpp"
#include <nlohmann/json.hpp>
#include "ICommandBuilder.hpp"
#include <IoC.hpp>
#include "ICommandBuilder.hpp"

using json = nlohmann::json;

BattleController::BattleController(
    std::shared_ptr<GameManager> gameManager,
    std::shared_ptr<IJWTVerifier> jwtVerifier)
    : gameManager_(gameManager), jwtVerifier_(jwtVerifier) {}

void BattleController::registerRoutes(httplib::Server &server)
{
    auto self = shared_from_this();
    server.Post("/api/battle/command", [self](const httplib::Request& req, httplib::Response& res) {
        self->handleGameCommand(req, res);
    });
}

void BattleController::handleGameCommand(const httplib::Request &req, httplib::Response &res)
{
    try
    {
        // Извлекаем token
        auto authHeader = req.get_header_value("Authorization");
        if (authHeader.empty())
        {
            throw std::runtime_error("Missing Authorization header");
        }

        std::string token = authHeader;
        if (token.substr(0, 7) == "Bearer ")
        {
            token = token.substr(7);
        }

        // Верифицируем токен
        JWTPayload payload = jwtVerifier_->verify(token);

        // Парсим JSON
        auto body = json::parse(req.body);
        std::string commandName = body["command"];
        json params = body.value("params", json::object());

        // Resolve CommandBuilder из IoC по имени
        std::string builderKey = "CommandBuilder." + commandName;
        auto builder = IoC::resolve<ICommandBuilder>(builderKey);

        // Строим команду
        auto command = builder->build(params);

        // Добавляем команду в очередь игры
        auto queue = gameManager_->getOrCreate(payload.gameId);
        queue->push(command);

        json response;
        response["status"] = "success";
        response["userId"] = payload.userId;
        response["gameId"] = payload.gameId;
        response["command"] = commandName;
        response["message"] = "Command queued successfully";

        res.set_content(response.dump(), "application/json");
        res.status = 200;
    }
    catch (const std::exception &e)
    {
        json error;
        error["error"] = e.what();
        res.set_content(error.dump(), "application/json");
        res.status = 401;
    }
}
