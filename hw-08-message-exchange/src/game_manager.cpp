#include "game_manager.hpp"

std::shared_ptr<ThreadSafeQueue> GameManager::getOrCreate(const std::string& gameId) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = games_.find(gameId);
    if (it != games_.end()) return it->second;
    auto q = std::make_shared<ThreadSafeQueue>();
    games_[gameId] = q;
    return q;
}
