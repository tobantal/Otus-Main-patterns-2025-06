#include <gtest/gtest.h>
#include <httplib.h>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

#include "AuthApplication.hpp"
#include "BattleApplication.hpp"

using json = nlohmann::json;

class IntegrationTest : public ::testing::Test {
protected:
    static AuthApplication *authApp;
    static BattleApplication *battleApp;
    static std::thread authServerThread;
    static std::thread battleServerThread;

    static void SetUpTestSuite() {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "[INTEGRATION TEST SUITE] Starting SetUpTestSuite" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        authApp = new AuthApplication();
        battleApp = new BattleApplication();

        std::cout << "[SUITE] Creating Auth Service server thread..." << std::endl;
        authServerThread = std::thread([]() {
            authApp->run("127.0.0.1", 8095);
        });

        std::cout << "[SUITE] Creating Battle Service server thread..." << std::endl;
        battleServerThread = std::thread([]() {
            battleApp->run("127.0.0.1", 8090);
        });

        std::cout << "[SUITE] Waiting 3 seconds for servers to start..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "[SUITE] ✓ Servers ready" << std::endl << std::endl;
    }

    static void TearDownTestSuite() {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "[INTEGRATION TEST SUITE] Starting TearDownTestSuite" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        std::cout << "[SUITE] Stopping Auth Service..." << std::endl;
        if (authApp) authApp->stop();
        
        std::cout << "[SUITE] Stopping Battle Service..." << std::endl;
        if (battleApp) battleApp->stop();

        std::cout << "[SUITE] Waiting for Auth thread..." << std::endl;
        if (authServerThread.joinable()) authServerThread.join();
        
        std::cout << "[SUITE] Waiting for Battle thread..." << std::endl;
        if (battleServerThread.joinable()) battleServerThread.join();

        delete authApp;
        delete battleApp;
        
        std::cout << "[SUITE] ✓ Cleanup complete" << std::endl << std::endl;
    }
};

AuthApplication *IntegrationTest::authApp = nullptr;
BattleApplication *IntegrationTest::battleApp = nullptr;
std::thread IntegrationTest::authServerThread;
std::thread IntegrationTest::battleServerThread;

TEST_F(IntegrationTest, CreateGameAndIssueToken) {
    std::cout << "\n[TEST] ============ CreateGameAndIssueToken START ============" << std::endl;
    
    httplib::Client authClient("127.0.0.1", 8095);
    authClient.set_connection_timeout(5, 0);

    // STEP 1
    std::cout << "\n[TEST] STEP 1/2: Creating new game..." << std::endl;
    json createGameReq;
    createGameReq["organizerId"] = "org_123";
    createGameReq["participants"] = {"player_1", "player_2"};
    
    std::cout << "[TEST] → POST /api/auth/game" << std::endl;
    std::cout << "[TEST] → Request: " << createGameReq.dump() << std::endl;
    
    auto createRes = authClient.Post("/api/auth/game", createGameReq.dump(), "application/json");
    
    ASSERT_TRUE(createRes) << "Failed to connect to Auth Service";
    ASSERT_EQ(createRes->status, 200) << "Error: " << createRes->body;
    
    auto createBody = json::parse(createRes->body);
    std::string gameId = createBody["gameId"];
    
    std::cout << "[TEST] ✓ Game created successfully" << std::endl;
    std::cout << "[TEST] ✓ Game ID: " << gameId << std::endl;
    ASSERT_FALSE(gameId.empty());

    // STEP 2
    std::cout << "\n[TEST] STEP 2/2: Requesting JWT token for player_1..." << std::endl;
    json tokenReq;
    tokenReq["gameId"] = gameId;
    tokenReq["userId"] = "player_1";
    
    std::cout << "[TEST] → POST /api/auth/token" << std::endl;
    std::cout << "[TEST] → Request: " << tokenReq.dump() << std::endl;
    
    auto tokenRes = authClient.Post("/api/auth/token", tokenReq.dump(), "application/json");
    
    ASSERT_TRUE(tokenRes) << "Failed to get token";
    ASSERT_EQ(tokenRes->status, 200) << "Error: " << tokenRes->body;
    
    auto tokenBody = json::parse(tokenRes->body);
    std::string token = tokenBody["token"];
    
    std::cout << "[TEST] ✓ Token issued successfully" << std::endl;
    std::cout << "[TEST] ✓ Token length: " << token.length() << " bytes" << std::endl;
    ASSERT_FALSE(token.empty());
    
    std::cout << "\n[TEST] ============ CreateGameAndIssueToken SUCCESS ============" << std::endl;
}

TEST_F(IntegrationTest, SendMoveCommandWithValidToken) {
    std::cout << "\n[TEST] ============ SendMoveCommandWithValidToken START ============" << std::endl;
    
    httplib::Client authClient("127.0.0.1", 8095);
    httplib::Client battleClient("127.0.0.1", 8090);
    authClient.set_connection_timeout(5, 0);
    battleClient.set_connection_timeout(5, 0);

    // STEP 1
    std::cout << "\n[TEST] STEP 1/3: Creating game on Auth Service..." << std::endl;
    json createGameReq;
    createGameReq["organizerId"] = "org_456";
    createGameReq["participants"] = {"player_3", "player_4"};
    
    std::cout << "[TEST] → POST /api/auth/game" << std::endl;
    auto createRes = authClient.Post("/api/auth/game", createGameReq.dump(), "application/json");
    ASSERT_TRUE(createRes);
    ASSERT_EQ(createRes->status, 200) << "Game creation failed: " << createRes->body;
    
    auto createBody = json::parse(createRes->body);
    std::string gameId = createBody["gameId"];
    std::cout << "[TEST] ✓ Game created: " << gameId << std::endl;

    // STEP 2
    std::cout << "\n[TEST] STEP 2/3: Getting JWT token for player_3..." << std::endl;
    json tokenReq;
    tokenReq["gameId"] = gameId;
    tokenReq["userId"] = "player_3";
    
    std::cout << "[TEST] → POST /api/auth/token" << std::endl;
    auto tokenRes = authClient.Post("/api/auth/token", tokenReq.dump(), "application/json");
    ASSERT_TRUE(tokenRes);
    ASSERT_EQ(tokenRes->status, 200) << "Token request failed: " << tokenRes->body;
    
    auto tokenBody = json::parse(tokenRes->body);
    std::string token = tokenBody["token"];
    std::cout << "[TEST] ✓ Token received: " << token.substr(0, 20) << "..." << std::endl;

    // STEP 3
    std::cout << "\n[TEST] STEP 3/3: Sending Move command to Battle Service..." << std::endl;
    json commandReq;
    commandReq["command"] = "Move";
    commandReq["params"]["x"] = 100;
    commandReq["params"]["y"] = 200;
    
    httplib::Headers headers;
    headers.insert({"Authorization", "Bearer " + token});
    
    std::cout << "[TEST] → POST /api/battle/command" << std::endl;
    std::cout << "[TEST] → Command: " << commandReq.dump() << std::endl;
    std::cout << "[TEST] → Authorization: Bearer " << token.substr(0, 20) << "..." << std::endl;
    
    auto cmdRes = battleClient.Post("/api/battle/command", headers, commandReq.dump(), "application/json");
    
    ASSERT_TRUE(cmdRes) << "Failed to connect to Battle Service";
    ASSERT_EQ(cmdRes->status, 200) << "Command failed: " << cmdRes->body;
    
    auto cmdBody = json::parse(cmdRes->body);
    std::cout << "[TEST] ✓ Command accepted" << std::endl;
    std::cout << "[TEST] ✓ Response: " << cmdBody.dump() << std::endl;
    EXPECT_EQ(cmdBody["status"], "success");
    
    std::cout << "\n[TEST] ============ SendMoveCommandWithValidToken SUCCESS ============" << std::endl;
}

TEST_F(IntegrationTest, RejectCommandWithInvalidToken) {
    std::cout << "\n[TEST] ============ RejectCommandWithInvalidToken START ============" << std::endl;
    
    httplib::Client battleClient("127.0.0.1", 8090);
    battleClient.set_connection_timeout(5, 0);

    json commandReq;
    commandReq["command"] = "Move";
    commandReq["params"]["x"] = 50;
    commandReq["params"]["y"] = 60;

    httplib::Headers headers;
    std::string invalidToken = "invalid_token_xyz123";
    headers.insert({"Authorization", "Bearer " + invalidToken});

    std::cout << "\n[TEST] Sending command with INVALID token..." << std::endl;
    std::cout << "[TEST] → POST /api/battle/command" << std::endl;
    std::cout << "[TEST] → Authorization: Bearer " << invalidToken << std::endl;
    
    auto cmdRes = battleClient.Post("/api/battle/command", headers, commandReq.dump(), "application/json");

    ASSERT_TRUE(cmdRes);
    std::cout << "[TEST] ✓ Server response: HTTP " << cmdRes->status << std::endl;
    std::cout << "[TEST] ✓ Body: " << cmdRes->body << std::endl;
    EXPECT_EQ(cmdRes->status, 401) << "Server should reject invalid token";
    
    std::cout << "\n[TEST] ============ RejectCommandWithInvalidToken SUCCESS ============" << std::endl;
}

TEST_F(IntegrationTest, RejectCommandWithoutToken) {
    std::cout << "\n[TEST] ============ RejectCommandWithoutToken START ============" << std::endl;
    
    httplib::Client battleClient("127.0.0.1", 8090);
    battleClient.set_connection_timeout(5, 0);

    json commandReq;
    commandReq["command"] = "Rotate";
    commandReq["params"]["angle"] = 45;

    std::cout << "\n[TEST] Sending command WITHOUT authorization header..." << std::endl;
    std::cout << "[TEST] → POST /api/battle/command" << std::endl;
    std::cout << "[TEST] → NO Authorization header" << std::endl;
    
    auto cmdRes = battleClient.Post("/api/battle/command", commandReq.dump(), "application/json");

    ASSERT_TRUE(cmdRes);
    std::cout << "[TEST] ✓ Server response: HTTP " << cmdRes->status << std::endl;
    std::cout << "[TEST] ✓ Body: " << cmdRes->body << std::endl;
    EXPECT_EQ(cmdRes->status, 401) << "Server should require token";
    
    std::cout << "\n[TEST] ============ RejectCommandWithoutToken SUCCESS ============" << std::endl;
}

TEST_F(IntegrationTest, RejectTokenForNonParticipant) {
    std::cout << "\n[TEST] ============ RejectTokenForNonParticipant START ============" << std::endl;
    
    httplib::Client authClient("127.0.0.1", 8095);
    authClient.set_connection_timeout(5, 0);

    // STEP 1
    std::cout << "\n[TEST] STEP 1/2: Creating game with specific participants..." << std::endl;
    json createGameReq;
    createGameReq["organizerId"] = "org_special";
    createGameReq["participants"] = {"player_10", "player_11"};
    
    std::cout << "[TEST] → POST /api/auth/game" << std::endl;
    std::cout << "[TEST] → Participants: player_10, player_11" << std::endl;
    
    auto createRes = authClient.Post("/api/auth/game", createGameReq.dump(), "application/json");
    ASSERT_TRUE(createRes);
    ASSERT_EQ(createRes->status, 200) << createRes->body;

    auto createBody = json::parse(createRes->body);
    std::string gameId = createBody["gameId"];
    std::cout << "[TEST] ✓ Game created: " << gameId << std::endl;

    // STEP 2
    std::cout << "\n[TEST] STEP 2/2: Requesting token for NON-participant player_99..." << std::endl;
    json tokenReq;
    tokenReq["gameId"] = gameId;
    tokenReq["userId"] = "player_99";
    
    std::cout << "[TEST] → POST /api/auth/token" << std::endl;
    std::cout << "[TEST] → Requesting token for: player_99 (NOT in participants list)" << std::endl;
    
    auto tokenRes = authClient.Post("/api/auth/token", tokenReq.dump(), "application/json");

    ASSERT_TRUE(tokenRes);
    std::cout << "[TEST] ✓ Server response: HTTP " << tokenRes->status << std::endl;
    std::cout << "[TEST] ✓ Body: " << tokenRes->body << std::endl;
    EXPECT_EQ(tokenRes->status, 400) << "Server should reject non-participant";
    
    std::cout << "\n[TEST] ============ RejectTokenForNonParticipant SUCCESS ============" << std::endl;
}
