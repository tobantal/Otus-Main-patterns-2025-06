#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "ThreadSafeQueue.hpp"   // из hw-07

class GameManager {
public:
    // берем неконфликтный не-шаблонный ThreadSafeQueue
    std::shared_ptr<ThreadSafeQueue> getOrCreate(const std::string& gameId);

private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::shared_ptr<ThreadSafeQueue>> games_;
};
