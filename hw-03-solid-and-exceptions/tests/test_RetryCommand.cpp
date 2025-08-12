#include <gtest/gtest.h>
#include "RetryCommand.hpp"
#include "TestCommands.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief ����� ��� RetryCommand
 */
class RetryCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ��������� ����� ������ ������
    }
};

/**
 * @brief ���� �������� RetryCommand
 */
TEST_F(RetryCommandTest, CreateRetryCommand) {
    auto innerCmd = std::make_shared<SuccessCommand>("TestCommand");

    ASSERT_NO_THROW({
        RetryCommand retryCmd(innerCmd);
        EXPECT_EQ(retryCmd.getInnerCommand(), innerCmd);
        });
}

/**
 * @brief ���� �������� RetryCommand � null
 */
TEST_F(RetryCommandTest, CreateWithNullThrows) {
    EXPECT_THROW({
        RetryCommand retryCmd(nullptr);
        }, std::invalid_argument);
}

/**
 * @brief ���� ���������� �������� ������� ����� RetryCommand
 */
TEST_F(RetryCommandTest, ExecuteSuccessfulCommand) {
    auto successCmd = std::make_shared<SuccessCommand>("TestSuccess");
    RetryCommand retryCmd(successCmd);

    ASSERT_NO_THROW(retryCmd.execute());
    EXPECT_EQ(successCmd->getExecutionCount(), 1);
}

/**
 * @brief ���� ���������� ��������� ������� ����� RetryCommand
 */
TEST_F(RetryCommandTest, ExecuteFailingCommand) {
    auto failCmd = std::make_shared<FailCommand>("TestFail");
    RetryCommand retryCmd(failCmd);

    EXPECT_THROW(retryCmd.execute(), std::runtime_error);
    EXPECT_EQ(failCmd->getAttemptCount(), 1);
}

/**
 * @brief ���� ��������� ���������� �������
 */
TEST_F(RetryCommandTest, GetInnerCommand) {
    auto innerCmd = std::make_shared<SuccessCommand>("Inner");
    RetryCommand retryCmd(innerCmd);

    auto retrieved = retryCmd.getInnerCommand();
    EXPECT_EQ(retrieved, innerCmd);
}

/**
 * @brief ���� DoubleRetryCommand
 */
TEST_F(RetryCommandTest, DoubleRetryCommand) {
    auto innerCmd = std::make_shared<SuccessCommand>("TestDouble");

    ASSERT_NO_THROW({
        DoubleRetryCommand doubleRetry(innerCmd);
        EXPECT_EQ(doubleRetry.getInnerCommand(), innerCmd);
        doubleRetry.execute();
        });

    EXPECT_EQ(innerCmd->getExecutionCount(), 1);
}

/**
 * @brief ���� DoubleRetryCommand � null
 */
TEST_F(RetryCommandTest, DoubleRetryWithNullThrows) {
    EXPECT_THROW({
        DoubleRetryCommand doubleRetry(nullptr);
        }, std::invalid_argument);
}

/**
 * @brief ���� ��������� RetryCommand
 */
TEST_F(RetryCommandTest, NestedRetryCommands) {
    auto innerCmd = std::make_shared<SuccessCommand>("Nested");
    auto retry1 = std::make_shared<RetryCommand>(innerCmd);
    auto retry2 = std::make_shared<RetryCommand>(retry1);

    ASSERT_NO_THROW(retry2->execute());
    EXPECT_EQ(innerCmd->getExecutionCount(), 1);
}

/**
 * @brief ���� RetryableCommand � ������������
 */
TEST_F(RetryCommandTest, RetryableCommandBehavior) {
    auto retryableCmd = std::make_shared<RetryableCommand>("Retryable", 2);

    // ������ ������� - �������
    EXPECT_THROW(retryableCmd->execute(), std::runtime_error);
    EXPECT_FALSE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 1);

    // ������ ������� - �������
    EXPECT_THROW(retryableCmd->execute(), std::runtime_error);
    EXPECT_FALSE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 2);

    // ������ ������� - �����
    ASSERT_NO_THROW(retryableCmd->execute());
    EXPECT_TRUE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 3);
}