#include "http_server/httplib.h"
#include <nlohmann/json.hpp>
#include "game_manager.hpp"
#include "InterpretCommand.hpp"
#include "IocContainer.hpp"

int main() {
    GameManager manager;
    InterpretCommand interpreter(manager);
    auto& container = IocContainer::getInstance();

    httplib::Server svr;
    svr.Post("/game/command", [&](const httplib::Request& req, httplib::Response& res){
        try {
            auto msg = nlohmann::json::parse(req.body);
            // регистрируем JSON
            auto scope = container.createScope("req");
            container.setCurrentScope("req");
            scope->registerDependency(
                "incomingMessage",
                [msg]() { return std::make_shared<nlohmann::json>(msg); }
            );
            interpreter.execute();
            res.set_content(R"({"status":"accepted"})", "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"invalid_message"})", "application/json");
        }
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}
