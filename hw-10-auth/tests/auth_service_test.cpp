#include <gtest/gtest.h>
#include <httplib.h>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include "AuthApplication.hpp"

using json = nlohmann::json;

class AuthServiceTest : public ::testing::Test {
protected:
    static AuthApplication *authApp;
    static std::thread authServerThread;

    static void SetUpTestSuite() {
        std::cout << "[AUTH SERVICE TEST] SetUp" << std::endl;
        authApp = new AuthApplication();
        authServerThread = std::thread([]() {
            authApp->run("127.0.0.1", 8095);
        });
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    static void TearDownTestSuite() {
        std::cout << "[AUTH SERVICE TEST] TearDown" << std::endl;
        authApp->stop();
        if (authServerThread.joinable()) authServerThread.join();
        delete authApp;
    }
};

AuthApplication *AuthServiceTest::authApp = nullptr;
std::thread AuthServiceTest::authServerThread;

TEST_F(AuthServiceTest, CreateGameAndGetToken) {
    httplib::Client client("127.0.0.1", 8095);
    client.set_connection_timeout(5, 0);
    client.set_read_timeout(5, 0);
    
    json createReq;
    createReq["organizerId"] = "org_123";
    createReq["participants"] = {"player_1", "player_2"};
    
    auto createRes = client.Post("/api/auth/game", createReq.dump(), "application/json");
    ASSERT_TRUE(createRes);
    ASSERT_EQ(createRes->status, 200);
    ASSERT_FALSE(json::parse(createRes->body)["gameId"].is_null());
}

TEST_F(AuthServiceTest, RejectTokenForNonParticipant) {
    httplib::Client client("127.0.0.1", 8095);
    
    json createReq;
    createReq["organizerId"] = "org_456";
    createReq["participants"] = {"player_10"};
    
    auto createRes = client.Post("/api/auth/game", createReq.dump(), "application/json");
    auto gameId = json::parse(createRes->body)["gameId"];
    
    json tokenReq;
    tokenReq["gameId"] = gameId;
    tokenReq["userId"] = "player_99";  // НЕ участник
    
    auto tokenRes = client.Post("/api/auth/token", tokenReq.dump(), "application/json");
    ASSERT_TRUE(tokenRes);
    ASSERT_EQ(tokenRes->status, 400);  // Bad request
}
