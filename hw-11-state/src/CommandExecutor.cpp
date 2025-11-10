#include "CommandExecutor.hpp"
#include <stdexcept>

CommandExecutor::CommandExecutor(
    std::shared_ptr<ThreadSafeQueue> queue,
    std::shared_ptr<IExecutorState> initialState)
    : queue_(std::move(queue)), currentState_(std::move(initialState)) 
{
    if (!queue_) {
        throw std::invalid_argument("CommandExecutor: queue cannot be nullptr");
    }
}

CommandExecutor::~CommandExecutor() {
    setState(nullptr);
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}

void CommandExecutor::executionLoop() {
    while (currentState_) {
        auto command = queue_->pop();
        if (command) {
            currentState_->handle(command);
        }
    }
}

void CommandExecutor::start() {
    if (thread_ && thread_->joinable()) {
        throw std::runtime_error("CommandExecutor: thread already running");
    }
    thread_ = std::make_unique<std::thread>(&CommandExecutor::executionLoop, this);
}

void CommandExecutor::join() {
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}

void CommandExecutor::setState(std::shared_ptr<IExecutorState> state) {
    currentState_ = std::move(state);
}

std::shared_ptr<IExecutorState> CommandExecutor::getState() const {
    return currentState_;
}

std::shared_ptr<ThreadSafeQueue> CommandExecutor::getQueue() const {
    return queue_;
}
