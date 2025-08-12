#include <gtest/gtest.h>
#include <filesystem>
#include "CommandQueue.hpp"
#include "ExceptionHandlers.hpp"
#include "TestCommands.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief ����� ��� ������������ ����������
 */
class ExceptionHandlersTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue_ = std::make_unique<CommandQueue>();
        testLogFile_ = "test_handler.log";

        // ������� �������� ���� ����
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    std::unique_ptr<CommandQueue> queue_;
    std::string testLogFile_;
};

/**
 * @brief ���� LogExceptionHandler
 */
TEST_F(ExceptionHandlersTest, LogExceptionHandler) {
    LogExceptionHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // ���������� ������ �������� LogCommand � �������
    size_t sizeBefore = queue_->size();
    handler.handle(failCmd, error);
    size_t sizeAfter = queue_->size();

    EXPECT_EQ(sizeAfter, sizeBefore + 1);

    // ��������� ������� �� �������
    queue_->processOne();

    // ���������, ��� ��� ���� ������
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief ���� RetryExceptionHandler
 */
TEST_F(ExceptionHandlersTest, RetryExceptionHandler) {
    RetryExceptionHandler handler(queue_.get());

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // ���������� ������ �������� RetryCommand � �������
    size_t sizeBefore = queue_->size();
    handler.handle(failCmd, error);
    size_t sizeAfter = queue_->size();

    EXPECT_EQ(sizeAfter, sizeBefore + 1);
}

/**
 * @brief ���� RetryThenLogHandler - ������ �������
 */
TEST_F(ExceptionHandlersTest, RetryThenLogHandler_FirstAttempt) {
    RetryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // ������ ������� - ������ �������� RetryCommand
    handler.handle(failCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief ���� RetryThenLogHandler - ������ �������
 */
TEST_F(ExceptionHandlersTest, RetryThenLogHandler_SecondAttempt) {
    RetryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto retryCmd = std::make_shared<RetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // ������ ������� (RetryCommand) - ������ �������� LogCommand
    handler.handle(retryCmd, error);
    EXPECT_EQ(queue_->size(), 1);

    // ��������� � ���������, ��� ������ ���
    queue_->processOne();
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief ���� DoubleTryThenLogHandler - ������ �������
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_FirstAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // ������ ������� - ������ �������� RetryCommand
    handler.handle(failCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief ���� DoubleTryThenLogHandler - ������ �������
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_SecondAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto retryCmd = std::make_shared<RetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // ������ ������� - ������ �������� DoubleRetryCommand
    handler.handle(retryCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief ���� DoubleTryThenLogHandler - ������ �������
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_ThirdAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto doubleRetryCmd = std::make_shared<DoubleRetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // ������ ������� - ������ �������� LogCommand
    handler.handle(doubleRetryCmd, error);
    EXPECT_EQ(queue_->size(), 1);

    // ��������� � ���������, ��� ������ ���
    queue_->processOne();
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief ���� ������� ����� RetryThenLog
 */
TEST_F(ExceptionHandlersTest, RetryThenLog_FullCycle) {
    auto handler = std::make_shared<RetryThenLogHandler>(queue_.get(), testLogFile_);

    // ������������ �����������
    queue_->registerHandler<FailCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);

    // ��������� �������, ������� ������ ������ �������
    auto failCmd = std::make_shared<FailCommand>("AlwaysFail");
    queue_->push(failCmd);

    // ������������ ��� �������
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // ���������, ��� ������� ���� ������� 2 ����
    EXPECT_EQ(failCmd->getAttemptCount(), 2);

    // ���������, ��� ������ ��� ����
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief ���� ������� ����� DoubleTryThenLog
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLog_FullCycle) {
    auto handler = std::make_shared<DoubleTryThenLogHandler>(queue_.get(), testLogFile_);

    // ������������ �����������
    queue_->registerHandler<FailCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);
    queue_->registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // ��������� �������, ������� ������ ������ �������
    auto failCmd = std::make_shared<FailCommand>("AlwaysFail");
    queue_->push(failCmd);

    // ������������ ��� �������
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // ���������, ��� ������� ���� ������� 3 ����
    EXPECT_EQ(failCmd->getAttemptCount(), 3);

    // ���������, ��� ������ ��� ����
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief ���� ��������� ���������� ����� ����������
 */
TEST_F(ExceptionHandlersTest, SuccessfulRetryAfterFailures) {
    auto handler = std::make_shared<DoubleTryThenLogHandler>(queue_.get(), testLogFile_);

    // ������������ �����������
    queue_->registerHandler<RetryableCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);
    queue_->registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // �������, ������� ������� ����� 2 ������
    auto retryableCmd = std::make_shared<RetryableCommand>("EventualSuccess", 2);
    queue_->push(retryableCmd);

    // ������������ ��� �������
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // ���������, ��� ������� ���� �������
    EXPECT_TRUE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 3);

    // ��� ���� �� ������ ���� ������, ��� ��� ������� �������
    EXPECT_FALSE(std::filesystem::exists(testLogFile_));
}