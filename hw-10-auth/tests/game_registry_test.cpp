#include <gtest/gtest.h>
#include "GameRegistry.hpp"
#include <thread>
#include <vector>

class GameRegistryTest : public ::testing::Test {
protected:
    GameRegistry registry;
};

TEST_F(GameRegistryTest, CreateGameBasic) {
    std::vector<std::string> participants = {"player1", "player2"};
    auto gameId = registry.createGame("org123", participants, std::chrono::hours(1));
    
    EXPECT_FALSE(gameId.empty());
    EXPECT_EQ(gameId.find("game_"), 0);  // Начинается с "game_"
}

TEST_F(GameRegistryTest, GetGameAfterCreate) {
    std::vector<std::string> participants = {"player1", "player2"};
    auto gameId = registry.createGame("org123", participants, std::chrono::hours(1));
    
    auto game = registry.getGame(gameId);
    ASSERT_NE(game, nullptr);
    EXPECT_EQ(game->gameId, gameId);
    EXPECT_EQ(game->organizerId, "org123");
    EXPECT_EQ(game->participants.size(), 2);
}

TEST_F(GameRegistryTest, GetNonExistentGame) {
    auto game = registry.getGame("nonexistent");
    EXPECT_EQ(game, nullptr);
}

TEST_F(GameRegistryTest, IsParticipant) {
    std::vector<std::string> participants = {"player1", "player2"};
    auto gameId = registry.createGame("org123", participants, std::chrono::hours(1));
    
    EXPECT_TRUE(registry.isParticipant(gameId, "player1"));
    EXPECT_TRUE(registry.isParticipant(gameId, "player2"));
    EXPECT_FALSE(registry.isParticipant(gameId, "player3"));
}

TEST_F(GameRegistryTest, IsParticipantNonExistentGame) {
    auto result = registry.isParticipant("nonexistent", "player1");
    EXPECT_FALSE(result);
}

TEST_F(GameRegistryTest, MultipleGamesCreation) {
    const int NUM_GAMES = 50;
    std::vector<std::string> gameIds;
    
    for (int i = 0; i < NUM_GAMES; ++i) {
        auto gameId = registry.createGame("org" + std::to_string(i), {"p1", "p2"}, std::chrono::hours(1));
        gameIds.push_back(gameId);
    }
    
    // Проверяем все игры
    for (const auto& gameId : gameIds) {
        auto game = registry.getGame(gameId);
        ASSERT_NE(game, nullptr) << "Game not found: " << gameId;
        EXPECT_EQ(game->gameId, gameId);
    }
}

TEST_F(GameRegistryTest, ConcurrentCreateAndGet) {
    const int NUM_THREADS = 10;
    const int GAMES_PER_THREAD = 20;
    std::vector<std::thread> threads;
    std::vector<std::string> allGameIds;
    std::mutex ids_mutex;
    
    // Каждый поток создаёт свои игры
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([this, t, &allGameIds, &ids_mutex]() {
            for (int i = 0; i < GAMES_PER_THREAD; ++i) {
                std::vector<std::string> participants = {"p1", "p2"};
                auto gameId = registry.createGame("org_t" + std::to_string(t) + "_i" + std::to_string(i), 
                                                   participants, 
                                                   std::chrono::hours(1));
                
                std::unique_lock<std::mutex> lock(ids_mutex);
                allGameIds.push_back(gameId);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Проверяем что все игры созданы и доступны
    EXPECT_EQ(allGameIds.size(), NUM_THREADS * GAMES_PER_THREAD);
    
    for (const auto& gameId : allGameIds) {
        auto game = registry.getGame(gameId);
        ASSERT_NE(game, nullptr) << "Missing game: " << gameId;
        EXPECT_EQ(game->gameId, gameId);
    }
}

TEST_F(GameRegistryTest, ConcurrentReadWrite) {
    // Создаём одну игру
    std::vector<std::string> participants = {"p1", "p2", "p3"};
    auto gameId = registry.createGame("org", participants, std::chrono::hours(1));
    
    std::vector<std::thread> threads;
    std::atomic<int> read_success_count(0);
    
    // 10 потоков одновременно читают одну игру
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([this, gameId, &read_success_count]() {
            for (int j = 0; j < 100; ++j) {
                auto game = registry.getGame(gameId);
                if (game) {
                    read_success_count++;
                }
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(read_success_count, 1000);  // 10 threads * 100 reads
}
