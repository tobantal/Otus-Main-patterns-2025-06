#include "TestCommands.hpp"
#include <iostream>

// ==================== SuccessCommand ====================

SuccessCommand::SuccessCommand(const std::string& name)
    : name_(name), executionCount_(0) {
}

void SuccessCommand::execute() {
    executionCount_++;
    std::cout << "[SuccessCommand] " << name_
        << " executed successfully (count: " << executionCount_ << ")" << std::endl;
}

const std::string& SuccessCommand::getName() const {
    return name_;
}

int SuccessCommand::getExecutionCount() const {
    return executionCount_;
}

// ==================== FailCommand ====================

FailCommand::FailCommand(const std::string& name, const std::string& errorMessage)
    : name_(name), errorMessage_(errorMessage), attemptCount_(0) {
}

void FailCommand::execute() {
    attemptCount_++;
    std::cout << "[FailCommand] " << name_
        << " attempting execution (attempt: " << attemptCount_ << ")" << std::endl;
    throw std::runtime_error(errorMessage_ + " [" + name_ + "]");
}

const std::string& FailCommand::getName() const {
    return name_;
}

int FailCommand::getAttemptCount() const {
    return attemptCount_;
}

// ==================== RetryableCommand ====================

RetryableCommand::RetryableCommand(const std::string& name, int failuresBeforeSuccess)
    : name_(name),
    failuresBeforeSuccess_(failuresBeforeSuccess),
    attemptCount_(0),
    successful_(false) {
}

void RetryableCommand::execute() {
    attemptCount_++;

    std::cout << "[RetryableCommand] " << name_
        << " attempting execution (attempt: " << attemptCount_ << ")" << std::endl;

    if (attemptCount_ <= failuresBeforeSuccess_) {
        throw std::runtime_error("Temporary failure for " + name_ +
            " (attempt " + std::to_string(attemptCount_.load()) +
            "/" + std::to_string(failuresBeforeSuccess_ + 1) + ")");
    }

    successful_ = true;
    std::cout << "[RetryableCommand] " << name_ << " succeeded!" << std::endl;
}

const std::string& RetryableCommand::getName() const {
    return name_;
}

int RetryableCommand::getAttemptCount() const {
    return attemptCount_;
}

bool RetryableCommand::isSuccessful() const {
    return successful_;
}

void RetryableCommand::reset() {
    attemptCount_ = 0;
    successful_ = false;
}