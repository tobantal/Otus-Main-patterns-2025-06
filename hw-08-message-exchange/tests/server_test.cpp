#include <gtest/gtest.h>
#include "http_server/httplib.h"
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

using json = nlohmann::json;

class HelloWorldServerTest : public ::testing::Test {
protected:
    std::unique_ptr<httplib::Server> server;
    std::thread server_thread;
    
    void SetUp() override {
        server = std::make_unique<httplib::Server>();
        
        // Регистрируем endpoint-ы (копируем из main.cpp)
        server->Get("/hello", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("Hello World!", "text/plain");
        });
        
        server->Get("/hello-json", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("{\"message\": \"Hello World!\"}", "application/json");
        });
        
        // Запускаем сервер в отдельном потоке на тестовом порту
        server_thread = std::thread([this]() {
            server->listen("localhost", 8081);
        });
        
        // Даём серверу время запуститься
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    void TearDown() override {
        server->stop();
        if (server_thread.joinable()) {
            server_thread.join();
        }
    }
};

// Тест GET /hello
TEST_F(HelloWorldServerTest, HelloEndpoint) {
    httplib::Client client("localhost", 8081);
    
    auto res = client.Get("/hello");
    
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, "Hello World!");
    EXPECT_EQ(res->get_header_value("Content-Type"), "text/plain");
}

// Тест GET /hello-json
TEST_F(HelloWorldServerTest, HelloJsonEndpoint) {
    httplib::Client client("localhost", 8081);
    
    auto res = client.Get("/hello-json");
    
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    
    // Проверяем JSON
    auto j = json::parse(res->body);
    EXPECT_EQ(j["message"], "Hello World!");
    EXPECT_EQ(res->get_header_value("Content-Type"), "application/json");
}

// Тест несуществующего endpoint
TEST_F(HelloWorldServerTest, NotFoundEndpoint) {
    httplib::Client client("localhost", 8081);
    
    auto res = client.Get("/nonexistent");
    
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 404);
}
