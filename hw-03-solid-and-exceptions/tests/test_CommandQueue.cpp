#include <gtest/gtest.h>
#include "CommandQueue.hpp"
#include "TestCommands.hpp"
#include "ExceptionHandlers.hpp"

#include <filesystem>

/**
 * @brief ����� ��� CommandQueue
 */
class CommandQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue_ = std::make_unique<CommandQueue>();
    }

    std::unique_ptr<CommandQueue> queue_;
};

/**
 * @brief ���� ������ �������
 */
TEST_F(CommandQueueTest, EmptyQueue) {
    EXPECT_TRUE(queue_->isEmpty());
    EXPECT_EQ(queue_->size(), 0);
    EXPECT_FALSE(queue_->processOne());
}

/**
 * @brief ���� ���������� ������
 */
TEST_F(CommandQueueTest, PushCommands) {
    auto cmd1 = std::make_shared<SuccessCommand>("Cmd1");
    auto cmd2 = std::make_shared<SuccessCommand>("Cmd2");

    queue_->push(cmd1);
    EXPECT_FALSE(queue_->isEmpty());
    EXPECT_EQ(queue_->size(), 1);

    queue_->push(cmd2);
    EXPECT_EQ(queue_->size(), 2);
}

/**
 * @brief ���� ��������� �������� ������
 */
TEST_F(CommandQueueTest, ProcessSuccessfulCommands) {
    auto cmd1 = std::make_shared<SuccessCommand>("Cmd1");
    auto cmd2 = std::make_shared<SuccessCommand>("Cmd2");

    queue_->push(cmd1);
    queue_->push(cmd2);

    EXPECT_TRUE(queue_->processOne());
    EXPECT_EQ(queue_->size(), 1);
    EXPECT_EQ(cmd1->getExecutionCount(), 1);

    EXPECT_TRUE(queue_->processOne());
    EXPECT_EQ(queue_->size(), 0);
    EXPECT_EQ(cmd2->getExecutionCount(), 1);
}

/**
 * @brief ���� ��������� ���� ������
 */
TEST_F(CommandQueueTest, ProcessAll) {
    auto cmd1 = std::make_shared<SuccessCommand>("Cmd1");
    auto cmd2 = std::make_shared<SuccessCommand>("Cmd2");
    auto cmd3 = std::make_shared<SuccessCommand>("Cmd3");

    queue_->push(cmd1);
    queue_->push(cmd2);
    queue_->push(cmd3);

    queue_->processAll();

    EXPECT_TRUE(queue_->isEmpty());
    EXPECT_EQ(cmd1->getExecutionCount(), 1);
    EXPECT_EQ(cmd2->getExecutionCount(), 1);
    EXPECT_EQ(cmd3->getExecutionCount(), 1);
}

/**
 * @brief ���� ��������� null �������
 */
TEST_F(CommandQueueTest, PushNullCommand) {
    queue_->push(nullptr);
    EXPECT_TRUE(queue_->isEmpty());
}

/**
 * @brief ���� ��������� ���������� ��� �����������
 */
TEST_F(CommandQueueTest, ExceptionWithoutHandler) {
    auto failCmd = std::make_shared<FailCommand>("Fail");
    queue_->push(failCmd);

    // ��� ����������� ���������� ������ ���� ����������
    EXPECT_THROW(queue_->processOne(), std::runtime_error);
}

/**
 * @brief ���� ����������� �� ���������
 */
TEST_F(CommandQueueTest, DefaultHandler) {
    auto logHandler = std::make_shared<LogExceptionHandler>(queue_.get(), "test.log");
    queue_->setDefaultHandler(logHandler);

    auto failCmd = std::make_shared<FailCommand>("Fail");
    queue_->push(failCmd);

    // � ������������ �� ��������� ���������� �� ������ ���� ����������
    ASSERT_NO_THROW(queue_->processOne());

    // ������ ���� ��������� LogCommand
    EXPECT_EQ(queue_->size(), 1);

    // ������� �������� ��� ����
    std::filesystem::remove("test.log");
}

/**
 * @brief ���� ����������� �������������� �����������
 */
TEST_F(CommandQueueTest, RegisterSpecificHandler) {
    auto retryHandler = std::make_shared<RetryExceptionHandler>(queue_.get());

    // ������������ ���������� ��� ���������� ���� (�������, ����������)
    queue_->registerHandler<FailCommand, std::runtime_error>(retryHandler);

    auto failCmd = std::make_shared<FailCommand>("Fail");
    queue_->push(failCmd);

    // ���������� ������ ���������
    ASSERT_NO_THROW(queue_->processOne());

    // ������ ���� ��������� RetryCommand
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief ���� ���������� �������������� ����������� ��� ������������ �� ���������
 */
TEST_F(CommandQueueTest, SpecificHandlerPriority) {
    auto logHandler = std::make_shared<LogExceptionHandler>(queue_.get(), "default.log");
    auto retryHandler = std::make_shared<RetryExceptionHandler>(queue_.get());

    // ������������� ���������� �� ���������
    queue_->setDefaultHandler(logHandler);

    // ������������ ������������� ����������
    queue_->registerHandler<FailCommand, std::runtime_error>(retryHandler);

    auto failCmd = std::make_shared<FailCommand>("Fail");
    queue_->push(failCmd);

    size_t sizeBefore = queue_->size();
    queue_->processOne();
    size_t sizeAfter = queue_->size();

    // ������ ��������� ������������� ���������� (RetryHandler),
    // ������� ���������� ����������� ������� FailCommand � ������� RetryCommand � �������
    EXPECT_EQ(sizeAfter, sizeBefore);

    // ������� �������� �����
    if (std::filesystem::exists("default.log")) {
        std::filesystem::remove("default.log");
    }
}

/**
 * @brief ���� ��������� ��������� �������� � ��������� ������
 */
TEST_F(CommandQueueTest, MixedCommandProcessing) {
    auto logHandler = std::make_shared<LogExceptionHandler>(queue_.get(), "mixed.log");
    queue_->setDefaultHandler(logHandler);

    queue_->push(std::make_shared<SuccessCommand>("Success1"));
    queue_->push(std::make_shared<FailCommand>("Fail1"));
    queue_->push(std::make_shared<SuccessCommand>("Success2"));
    queue_->push(std::make_shared<FailCommand>("Fail2"));

    // ������������ ��� �������
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // ������� ������ ���� ����� (��� ������� ����������)
    EXPECT_TRUE(queue_->isEmpty());

    // ������� �������� ����
    if (std::filesystem::exists("mixed.log")) {
        std::filesystem::remove("mixed.log");
    }
    
}