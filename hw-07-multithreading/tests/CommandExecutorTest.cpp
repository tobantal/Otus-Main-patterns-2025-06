#include <gtest/gtest.h>
#include "CommandExecutor.hpp"
#include "LegacyThreadSafeQueue.hpp"
#include <chrono>
#include <thread>
#include <atomic>

/**
 * @brief Тестовая команда для подсчета выполнений
 */
class CounterCommand : public ICommand {
private:
    std::atomic<int>& counter_;
    std::chrono::milliseconds delay_;

public:
    CounterCommand(std::atomic<int>& counter, 
                   std::chrono::milliseconds delay = std::chrono::milliseconds(0))
        : counter_(counter), delay_(delay) {}

    void execute() override {
        if (delay_.count() > 0) {
            std::this_thread::sleep_for(delay_);
        }
        counter_++;
    }
};

/**
 * @brief Тестовая команда, которая всегда выбрасывает исключение
 */
class FailCommand : public ICommand {
public:
    void execute() override {
        throw std::runtime_error("Intentional failure");
    }
};

/**
 * @brief Тестовый набор для CommandExecutor
 */
class CommandExecutorTest : public ::testing::Test {
protected:
    std::shared_ptr<LegacyThreadSafeQueue> queue;
    std::shared_ptr<CommandExecutor> executor;

    void SetUp() override {
        queue = std::make_shared<LegacyThreadSafeQueue>();
        executor = std::make_shared<CommandExecutor>(queue);
    }

    void TearDown() override {
        if (executor->isRunning()) {
            executor->hardStop();
        }
    }
};

/**
 * @test Тест создания исполнителя
 */
TEST_F(CommandExecutorTest, CreateExecutor) {
    ASSERT_NE(nullptr, executor);
    ASSERT_FALSE(executor->isRunning());
}

/**
 * @test Тест запуска и остановки потока
 */
TEST_F(CommandExecutorTest, StartAndStop) {
    executor->start();
    ASSERT_TRUE(executor->isRunning());

    // Даем потоку немного времени для стабилизации
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    executor->hardStop();
    ASSERT_FALSE(executor->isRunning());
}

/**
 * @test Тест выполнения команд
 */
TEST_F(CommandExecutorTest, ExecuteCommands) {
    std::atomic<int> counter{0};

    executor->start();
    ASSERT_TRUE(executor->isRunning());

    // Добавляем несколько команд
    for (int i = 0; i < 5; i++) {
        queue->push(std::make_shared<CounterCommand>(counter));
    }

    // Ждем выполнения
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    executor->hardStop();

    ASSERT_EQ(5, counter.load());
}

/**
 * @test Тест обработки исключений
 */
TEST_F(CommandExecutorTest, HandleExceptions) {
    std::atomic<int> counter{0};

    executor->start();

    // Добавляем команды, включая те, что выбрасывают исключения
    queue->push(std::make_shared<CounterCommand>(counter));
    queue->push(std::make_shared<FailCommand>());
    queue->push(std::make_shared<CounterCommand>(counter));
    queue->push(std::make_shared<FailCommand>());
    queue->push(std::make_shared<CounterCommand>(counter));

    // Ждем выполнения
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    executor->hardStop();

    // Несмотря на исключения, все нормальные команды должны выполниться
    ASSERT_EQ(3, counter.load());
}

/**
 * @test Тест повторного запуска
 */
TEST_F(CommandExecutorTest, CannotStartTwice) {
    executor->start();
    ASSERT_TRUE(executor->isRunning());

    ASSERT_THROW(executor->start(), std::runtime_error);

    executor->hardStop();
}

/**
 * @test Тест создания с nullptr
 */
TEST(CommandExecutorCreationTest, ThrowsOnNullQueue) {
    ASSERT_THROW(CommandExecutor(nullptr), std::invalid_argument);
}