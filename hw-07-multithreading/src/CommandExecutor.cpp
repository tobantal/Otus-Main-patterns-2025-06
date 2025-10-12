#include "CommandExecutor.hpp"
#include <iostream>

CommandExecutor::CommandExecutor(std::shared_ptr<ThreadSafeQueue> queue)
    : queue_(queue)
    , thread_(nullptr)
    , running_(false)
    , softStop_(false) {
    
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

CommandExecutor::~CommandExecutor() {
    if (running_) {
        hardStop();
    }
}

void CommandExecutor::start() {
    if (running_) {
        throw std::runtime_error("Executor is already running");
    }

    running_ = true;
    softStop_ = false;
    
    thread_ = std::make_unique<std::thread>([this]() {
        executionLoop();
    });
}

void CommandExecutor::executionLoop() {
    while (running_) {
        std::shared_ptr<ICommand> command;
        
        // Если установлен soft stop, используем tryPop
        if (softStop_) {
            if (!queue_->tryPop(command)) {
                // Очередь пуста, можем завершаться
                break;
            }
        } else {
            // Блокирующее извлечение команды
            if (!queue_->pop(command)) {
                // Очередь закрыта
                break;
            }
        }

        if (command) {
            try {
                command->execute();
            } catch (const std::exception& e) {
                // Логируем ошибку, но не прерываем выполнение потока
                std::cerr << "[CommandExecutor] Exception caught: " 
                          << e.what() << std::endl;
            } catch (...) {
                std::cerr << "[CommandExecutor] Unknown exception caught" 
                          << std::endl;
            }
        }
    }

    running_ = false;
}

void CommandExecutor::hardStop() {
    if (!running_) {
        return;
    }

    running_ = false;
    queue_->shutdown();

    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}

void CommandExecutor::softStop() {
    if (!running_) {
        return;
    }

    softStop_ = true;
}

bool CommandExecutor::isRunning() const {
    return running_;
}

void CommandExecutor::join() {
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}

std::shared_ptr<ThreadSafeQueue> CommandExecutor::getQueue() const {
    return queue_;
}