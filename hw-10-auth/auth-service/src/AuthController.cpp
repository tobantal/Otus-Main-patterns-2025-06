#include "AuthController.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

AuthController::AuthController(
    std::shared_ptr<GameRegistry> registry,
    std::shared_ptr<IJWTGenerator> jwtGenerator)
    : registry_(registry), jwtGenerator_(jwtGenerator) {}

void AuthController::registerRoutes(httplib::Server& server) {
    auto self = shared_from_this();
    
    server.Post("/api/auth/game", [self](const httplib::Request& req, httplib::Response& res) {
        self->handleCreateGame(req, res);
    });

    server.Post("/api/auth/token", [self](const httplib::Request& req, httplib::Response& res) {
        self->handleIssueToken(req, res);
    });
}

void AuthController::handleCreateGame(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[handleCreateGame] START" << std::endl;
    
    try {
        std::cout << "[handleCreateGame] Parsing body: " << req.body << std::endl;
        auto body = json::parse(req.body);
        std::cout << "[handleCreateGame] Parsed successfully" << std::endl;

        std::string organizerId = body["organizerId"];
        std::cout << "[handleCreateGame] organizerId: " << organizerId << std::endl;
        
        std::vector<std::string> participants = body["participants"].get<std::vector<std::string>>();
        std::cout << "[handleCreateGame] participants parsed" << std::endl;

        std::cout << "[handleCreateGame] Calling createGame..." << std::endl;
        std::cout << "[handleCreateGame] registry_ is nullptr? " << (registry_ == nullptr) << std::endl;
        auto gameId = registry_->createGame(organizerId, participants, std::chrono::hours(1));
        std::cout << "[handleCreateGame] createGame returned: " << gameId << std::endl;

        json response;
        response["gameId"] = gameId;
        response["status"] = "success";

        std::cout << "[handleCreateGame] Setting response" << std::endl;
        res.set_content(response.dump(), "application/json");
        res.status = 200;
        std::cout << "[handleCreateGame] END - success" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "[handleCreateGame] EXCEPTION: " << e.what() << std::endl;
        json error;
        error["error"] = e.what();
        res.set_content(error.dump(), "application/json");
        res.status = 400;
        std::cout << "[handleCreateGame] END - error" << std::endl;
    }
}

void AuthController::handleIssueToken(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[Token] Starting handleIssueToken" << std::endl;
    
    try {
        std::cout << "[Token] Parsing request body" << std::endl;
        auto body = json::parse(req.body);

        std::string gameId = body["gameId"];
        std::string userId = body["userId"];
        
        std::cout << "[Token] gameId=" << gameId << ", userId=" << userId << std::endl;

        // Проверяем участника
        std::cout << "[Token] Checking if user is participant" << std::endl;
        if (!registry_->isParticipant(gameId, userId)) {
            throw std::runtime_error("User is not a participant of this game");
        }
        
        std::cout << "[Token] User is participant, generating token" << std::endl;

        // Генерируем payload
        JWTPayload payload;
        payload.userId = userId;
        payload.gameId = gameId;
        payload.role = "participant";
        payload.sub = userId;
        payload.iss = "auth-service";

        // Выдаём токен
        std::cout << "[Token] Calling jwtGenerator_->generate" << std::endl;
        auto token = jwtGenerator_->generate(payload, std::chrono::hours(24));
        
        std::cout << "[Token] Token generated successfully" << std::endl;

        json response;
        response["token"] = token;
        response["status"] = "success";

        res.set_content(response.dump(), "application/json");
        res.status = 200;
        
        std::cout << "[Token] Response sent" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "[Token] Exception: " << e.what() << std::endl;
        json error;
        error["error"] = e.what();
        res.set_content(error.dump(), "application/json");
        res.status = 400;
    }
}
