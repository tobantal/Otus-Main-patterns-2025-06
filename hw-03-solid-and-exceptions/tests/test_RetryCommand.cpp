#include <gtest/gtest.h>
#include "RetryCommand.hpp"
#include "TestCommands.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief Тесты для RetryCommand
 */
class RetryCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка перед каждым тестом
    }
};

/**
 * @brief Тест создания RetryCommand
 */
TEST_F(RetryCommandTest, CreateRetryCommand) {
    auto innerCmd = std::make_shared<SuccessCommand>("TestCommand");

    ASSERT_NO_THROW({
        RetryCommand retryCmd(innerCmd);
        EXPECT_EQ(retryCmd.getInnerCommand(), innerCmd);
        });
}

/**
 * @brief Тест создания RetryCommand с null
 */
TEST_F(RetryCommandTest, CreateWithNullThrows) {
    EXPECT_THROW({
        RetryCommand retryCmd(nullptr);
        }, std::invalid_argument);
}

/**
 * @brief Тест выполнения успешной команды через RetryCommand
 */
TEST_F(RetryCommandTest, ExecuteSuccessfulCommand) {
    auto successCmd = std::make_shared<SuccessCommand>("TestSuccess");
    RetryCommand retryCmd(successCmd);

    ASSERT_NO_THROW(retryCmd.execute());
    EXPECT_EQ(successCmd->getExecutionCount(), 1);
}

/**
 * @brief Тест выполнения неудачной команды через RetryCommand
 */
TEST_F(RetryCommandTest, ExecuteFailingCommand) {
    auto failCmd = std::make_shared<FailCommand>("TestFail");
    RetryCommand retryCmd(failCmd);

    EXPECT_THROW(retryCmd.execute(), std::runtime_error);
    EXPECT_EQ(failCmd->getAttemptCount(), 1);
}

/**
 * @brief Тест получения внутренней команды
 */
TEST_F(RetryCommandTest, GetInnerCommand) {
    auto innerCmd = std::make_shared<SuccessCommand>("Inner");
    RetryCommand retryCmd(innerCmd);

    auto retrieved = retryCmd.getInnerCommand();
    EXPECT_EQ(retrieved, innerCmd);
}

/**
 * @brief Тест DoubleRetryCommand
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
 * @brief Тест DoubleRetryCommand с null
 */
TEST_F(RetryCommandTest, DoubleRetryWithNullThrows) {
    EXPECT_THROW({
        DoubleRetryCommand doubleRetry(nullptr);
        }, std::invalid_argument);
}

/**
 * @brief Тест вложенных RetryCommand
 */
TEST_F(RetryCommandTest, NestedRetryCommands) {
    auto innerCmd = std::make_shared<SuccessCommand>("Nested");
    auto retry1 = std::make_shared<RetryCommand>(innerCmd);
    auto retry2 = std::make_shared<RetryCommand>(retry1);

    ASSERT_NO_THROW(retry2->execute());
    EXPECT_EQ(innerCmd->getExecutionCount(), 1);
}

/**
 * @brief Тест RetryableCommand с повторениями
 */
TEST_F(RetryCommandTest, RetryableCommandBehavior) {
    auto retryableCmd = std::make_shared<RetryableCommand>("Retryable", 2);

    // Первая попытка - неудача
    EXPECT_THROW(retryableCmd->execute(), std::runtime_error);
    EXPECT_FALSE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 1);

    // Вторая попытка - неудача
    EXPECT_THROW(retryableCmd->execute(), std::runtime_error);
    EXPECT_FALSE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 2);

    // Третья попытка - успех
    ASSERT_NO_THROW(retryableCmd->execute());
    EXPECT_TRUE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 3);
}