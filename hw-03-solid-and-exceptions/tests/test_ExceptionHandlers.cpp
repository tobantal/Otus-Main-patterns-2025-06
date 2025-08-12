#include <gtest/gtest.h>
#include <filesystem>
#include "CommandQueue.hpp"
#include "ExceptionHandlers.hpp"
#include "TestCommands.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief Тесты для обработчиков исключений
 */
class ExceptionHandlersTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue_ = std::make_unique<CommandQueue>();
        testLogFile_ = "test_handler.log";

        // Удаляем тестовый файл лога
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
 * @brief Тест LogExceptionHandler
 */
TEST_F(ExceptionHandlersTest, LogExceptionHandler) {
    LogExceptionHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // Обработчик должен добавить LogCommand в очередь
    size_t sizeBefore = queue_->size();
    handler.handle(failCmd, error);
    size_t sizeAfter = queue_->size();

    EXPECT_EQ(sizeAfter, sizeBefore + 1);

    // Выполняем команду из очереди
    queue_->processOne();

    // Проверяем, что лог файл создан
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief Тест RetryExceptionHandler
 */
TEST_F(ExceptionHandlersTest, RetryExceptionHandler) {
    RetryExceptionHandler handler(queue_.get());

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // Обработчик должен добавить RetryCommand в очередь
    size_t sizeBefore = queue_->size();
    handler.handle(failCmd, error);
    size_t sizeAfter = queue_->size();

    EXPECT_EQ(sizeAfter, sizeBefore + 1);
}

/**
 * @brief Тест RetryThenLogHandler - первая попытка
 */
TEST_F(ExceptionHandlersTest, RetryThenLogHandler_FirstAttempt) {
    RetryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // Первая попытка - должна добавить RetryCommand
    handler.handle(failCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief Тест RetryThenLogHandler - вторая попытка
 */
TEST_F(ExceptionHandlersTest, RetryThenLogHandler_SecondAttempt) {
    RetryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto retryCmd = std::make_shared<RetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // Вторая попытка (RetryCommand) - должна добавить LogCommand
    handler.handle(retryCmd, error);
    EXPECT_EQ(queue_->size(), 1);

    // Выполняем и проверяем, что создан лог
    queue_->processOne();
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief Тест DoubleTryThenLogHandler - первая попытка
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_FirstAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    std::runtime_error error("Test error");

    // Первая попытка - должна добавить RetryCommand
    handler.handle(failCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief Тест DoubleTryThenLogHandler - вторая попытка
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_SecondAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto retryCmd = std::make_shared<RetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // Вторая попытка - должна добавить DoubleRetryCommand
    handler.handle(retryCmd, error);
    EXPECT_EQ(queue_->size(), 1);
}

/**
 * @brief Тест DoubleTryThenLogHandler - третья попытка
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLogHandler_ThirdAttempt) {
    DoubleTryThenLogHandler handler(queue_.get(), testLogFile_);

    auto failCmd = std::make_shared<FailCommand>("TestFail");
    auto doubleRetryCmd = std::make_shared<DoubleRetryCommand>(failCmd);
    std::runtime_error error("Test error");

    // Третья попытка - должна добавить LogCommand
    handler.handle(doubleRetryCmd, error);
    EXPECT_EQ(queue_->size(), 1);

    // Выполняем и проверяем, что создан лог
    queue_->processOne();
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief Тест полного цикла RetryThenLog
 */
TEST_F(ExceptionHandlersTest, RetryThenLog_FullCycle) {
    auto handler = std::make_shared<RetryThenLogHandler>(queue_.get(), testLogFile_);

    // Регистрируем обработчики
    queue_->registerHandler<FailCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);

    // Добавляем команду, которая всегда терпит неудачу
    auto failCmd = std::make_shared<FailCommand>("AlwaysFail");
    queue_->push(failCmd);

    // Обрабатываем все команды
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // Проверяем, что команда была вызвана 2 раза
    EXPECT_EQ(failCmd->getAttemptCount(), 2);

    // Проверяем, что создан лог файл
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief Тест полного цикла DoubleTryThenLog
 */
TEST_F(ExceptionHandlersTest, DoubleTryThenLog_FullCycle) {
    auto handler = std::make_shared<DoubleTryThenLogHandler>(queue_.get(), testLogFile_);

    // Регистрируем обработчики
    queue_->registerHandler<FailCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);
    queue_->registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // Добавляем команду, которая всегда терпит неудачу
    auto failCmd = std::make_shared<FailCommand>("AlwaysFail");
    queue_->push(failCmd);

    // Обрабатываем все команды
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // Проверяем, что команда была вызвана 3 раза
    EXPECT_EQ(failCmd->getAttemptCount(), 3);

    // Проверяем, что создан лог файл
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));
}

/**
 * @brief Тест успешного выполнения после повторений
 */
TEST_F(ExceptionHandlersTest, SuccessfulRetryAfterFailures) {
    auto handler = std::make_shared<DoubleTryThenLogHandler>(queue_.get(), testLogFile_);

    // Регистрируем обработчики
    queue_->registerHandler<RetryableCommand, std::runtime_error>(handler);
    queue_->registerHandler<RetryCommand, std::runtime_error>(handler);
    queue_->registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // Команда, которая успешна после 2 неудач
    auto retryableCmd = std::make_shared<RetryableCommand>("EventualSuccess", 2);
    queue_->push(retryableCmd);

    // Обрабатываем все команды
    int maxIterations = 10;
    while (!queue_->isEmpty() && maxIterations-- > 0) {
        queue_->processOne();
    }

    // Проверяем, что команда была успешна
    EXPECT_TRUE(retryableCmd->isSuccessful());
    EXPECT_EQ(retryableCmd->getAttemptCount(), 3);

    // Лог файл не должен быть создан, так как команда успешна
    EXPECT_FALSE(std::filesystem::exists(testLogFile_));
}