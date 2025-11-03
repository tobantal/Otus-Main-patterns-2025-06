#include <gtest/gtest.h>
#include <httplib.h>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

#include "BattleApplication.hpp"
#include "RSAJWTGenerator.hpp"

using json = nlohmann::json;

class BattleServiceTest : public ::testing::Test {
protected:
    static BattleApplication *battleApp;
    static std::thread battleServerThread;
    static std::string privateKeyPath_;
    static std::string publicKeyPath_;
    
    void SetUp() override {}

    static void SetUpTestSuite() {
        std::cout << "[BattleServiceTest] Starting server..." << std::endl;
        
        battleApp = new BattleApplication();
        
        battleServerThread = std::thread([]() {
            battleApp->run("127.0.0.1", 8090);
        });
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[BattleServiceTest] Server ready" << std::endl;
    }

    static void TearDownTestSuite() {
        std::cout << "[BattleServiceTest] Stopping server..." << std::endl;
        
        if (battleApp) battleApp->stop();
        if (battleServerThread.joinable()) battleServerThread.join();
        
        delete battleApp;
        
        std::cout << "[BattleServiceTest] Stopped" << std::endl;
    }
};

BattleApplication *BattleServiceTest::battleApp = nullptr;
std::thread BattleServiceTest::battleServerThread;
std::string BattleServiceTest::privateKeyPath_ = "keys/private_key.pem";
std::string BattleServiceTest::publicKeyPath_ = "keys/public_key.pem";

TEST_F(BattleServiceTest, ValidToken_ReceivesCommand) {
    httplib::Client client("127.0.0.1", 8090);
    client.set_connection_timeout(5, 0);

    // Генерируем токен
    RSAJWTGenerator generator(privateKeyPath_);
    JWTPayload payload;
    payload.userId = "user_123";
    payload.gameId = "game_456";
    std::string token = generator.generate(payload);

    json commandReq;
    commandReq["command"] = "Move";
    commandReq["params"]["x"] = 100;
    commandReq["params"]["y"] = 200;

    httplib::Headers headers;
    headers.insert({"Authorization", "Bearer " + token});

    auto res = client.Post("/api/battle/command", headers, 
                           commandReq.dump(), "application/json");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    
    auto body = json::parse(res->body);
    EXPECT_EQ(body["status"], "success");
}

TEST_F(BattleServiceTest, InvalidToken_Rejected) {
    httplib::Client client("127.0.0.1", 8090);
    client.set_connection_timeout(5, 0);

    json commandReq;
    commandReq["command"] = "Move";

    httplib::Headers headers;
    headers.insert({"Authorization", "Bearer invalid_token_xyz"});

    auto res = client.Post("/api/battle/command", headers,
                           commandReq.dump(), "application/json");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 401);
}

TEST_F(BattleServiceTest, NoToken_Rejected) {
    httplib::Client client("127.0.0.1", 8090);
    client.set_connection_timeout(5, 0);

    json commandReq;
    commandReq["command"] = "Move";

    auto res = client.Post("/api/battle/command",
                           commandReq.dump(), "application/json");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 401);
}
