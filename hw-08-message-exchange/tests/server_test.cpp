#include <gtest/gtest.h>
#include "http_server/httplib.h"
#include <nlohmann/json.hpp>
#include "game_manager.hpp"
#include "InterpretCommand.hpp"
#include "IocContainer.hpp"
#include "TestCommand.hpp"

using json = nlohmann::json;
using namespace httplib;

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Регистрация тестовой команды
        IocContainer::getInstance().registerGlobalDependency(
            "test",
            []() { return std::make_shared<TestCommand>(); }
        );

        // Инициализация менеджера и интерпретатора
        interpreter_ = std::make_unique<InterpretCommand>(manager_);

        // Настройка сервера
        server_ = std::make_unique<Server>();
        server_->Post("/game/command", [&](const Request& req, Response& res){
            try {
                auto j = json::parse(req.body);
                auto& c = IocContainer::getInstance();
                auto scope = c.createScope("req");
                c.setCurrentScope("req");
                scope->registerDependency(
                    "incomingMessage",
                    [j]() { return std::make_shared<json>(j); }
                );
                interpreter_->execute();
                res.set_content(R"({"status":"accepted"})", "application/json");
            } catch (...) {
                res.status = 400;
                res.set_content(R"({"error":"invalid_message"})", "application/json");
            }
        });

        // Запуск в отдельном потоке
        thread_ = std::thread([&]() {
            server_->listen("localhost", 8081);
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    void TearDown() override {
        server_->stop();
        if (thread_.joinable()) thread_.join();
    }

    GameManager manager_;
    std::unique_ptr<InterpretCommand> interpreter_;
    std::unique_ptr<Server> server_;
    std::thread thread_;
};

TEST_F(ServerTest, ValidCommand) {
    Client cli("localhost", 8081);

    // Формируем корректное сообщение
    json j = {
        {"gameId", "game123"},
        {"operationId", "test"}
    };
    auto res = cli.Post("/game/command", j.dump(), "application/json");

    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, R"({"status":"accepted"})");
}

TEST_F(ServerTest, MissingFields) {
    Client cli("localhost", 8081);

    // Отсутствует operationId
    json j = {
        {"gameId", "game123"}
    };
    auto res = cli.Post("/game/command", j.dump(), "application/json");

    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->status, 400);
    EXPECT_EQ(res->body, R"({"error":"invalid_message"})");
}

TEST_F(ServerTest, InvalidJson) {
    Client cli("localhost", 8081);

    // Некорректный JSON
    std::string invalid = "not a json";
    auto res = cli.Post("/game/command", invalid, "application/json");

    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->status, 400);
    EXPECT_EQ(res->body, R"({"error":"invalid_message"})");
}
