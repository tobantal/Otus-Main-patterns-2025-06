#include "ThreadSafeQueue.hpp"

ThreadSafeQueue::ThreadSafeQueue() 
    : shutdown_(false) {
}

void ThreadSafeQueue::push(std::shared_ptr<ICommand> command) {
    if (!command) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (shutdown_) {
            return;
        }
        queue_.push(command);
    }
    condVar_.notify_one();
}

bool ThreadSafeQueue::pop(std::shared_ptr<ICommand>& command) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    condVar_.wait(lock, [this]() { 
        return !queue_.empty() || shutdown_; 
    });

    if (shutdown_ && queue_.empty()) {
        return false;
    }

    command = queue_.front();
    queue_.pop();
    return true;
}

bool ThreadSafeQueue::tryPop(std::shared_ptr<ICommand>& command) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (queue_.empty()) {
        return false;
    }

    command = queue_.front();
    queue_.pop();
    return true;
}

bool ThreadSafeQueue::popWithTimeout(std::shared_ptr<ICommand>& command,
                                     std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    bool notEmpty = condVar_.wait_for(lock, timeout, [this]() {
        return !queue_.empty() || shutdown_;
    });

    if (!notEmpty || (shutdown_ && queue_.empty())) {
        return false;
    }

    command = queue_.front();
    queue_.pop();
    return true;
}

bool ThreadSafeQueue::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

size_t ThreadSafeQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

void ThreadSafeQueue::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
    }
    condVar_.notify_all();
}

bool ThreadSafeQueue::isShutdown() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_;
}