#include <gtest/gtest.h>
#include "CommandExecutor.hpp"
#include "LegacyThreadSafeQueue.hpp"
#include "StartCommand.hpp"
#include "HardStopCommand.hpp"
#include "SoftStopCommand.hpp"
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>

/**
 * @brief Тестовая команда с уведомлением о выполнении
 */
class NotifyCommand : public ICommand {
private:
    std::atomic<bool>& executed_;
    std::condition_variable& cv_;
    std::mutex& mutex_;

public:
    NotifyCommand(std::atomic<bool>& executed, 
                  std::condition_variable& cv,
                  std::mutex& mutex)
        : executed_(executed), cv_(cv), mutex_(mutex) {}

    void execute() override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            executed_ = true;
        }
        cv_.notify_one();
    }
};

/**
 * @brief Тестовая команда с задержкой
 */
class DelayCommand : public ICommand {
private:
    std::chrono::milliseconds delay_;
    std::atomic<int>& counter_;

public:
    DelayCommand(std::chrono::milliseconds delay, std::atomic<int>& counter)
        : delay_(delay), counter_(counter) {}

    void execute() override {
        std::this_thread::sleep_for(delay_);
        counter_++;
    }
};

/**
 * @brief Тестовый набор для команд управления потоком
 */
class ThreadCommandsTest : public ::testing::Test {
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
 * @test Проверка, что после команды start поток запущен
 * Используются условные события синхронизации (condition_variable)
 */
TEST_F(ThreadCommandsTest, StartCommandLaunchesThread) {
    // Синхронизация с использованием condition_variable
    std::atomic<bool> executed{false};
    std::condition_variable cv;
    std::mutex mutex;

    auto startCmd = std::make_shared<StartCommand>(executor);
    startCmd->execute();

    ASSERT_TRUE(executor->isRunning());

    // Добавляем команду с уведомлением
    queue->push(std::make_shared<NotifyCommand>(executed, cv, mutex));

    // Ожидаем выполнения команды с таймаутом
    std::unique_lock<std::mutex> lock(mutex);
    bool notified = cv.wait_for(lock, std::chrono::seconds(1), 
                                [&executed]() { return executed.load(); });

    ASSERT_TRUE(notified) << "Command was not executed in time";
    ASSERT_TRUE(executed.load());
    ASSERT_TRUE(executor->isRunning());

    executor->hardStop();
}

/**
 * @test Проверка, что после команды hard stop поток завершается
 */
TEST_F(ThreadCommandsTest, HardStopCommandStopsThread) {
    auto startCmd = std::make_shared<StartCommand>(executor);
    startCmd->execute();

    ASSERT_TRUE(executor->isRunning());

    // Добавляем команды с задержкой
    std::atomic<int> counter{0};
    for (int i = 0; i < 10; i++) {
        queue->push(std::make_shared<DelayCommand>(
            std::chrono::milliseconds(50), counter));
    }

    // Немедленная остановка
    auto stopCmd = std::make_shared<HardStopCommand>(executor);
    stopCmd->execute();

    ASSERT_FALSE(executor->isRunning());

    // Не все команды должны быть выполнены
    ASSERT_LT(counter.load(), 10) 
        << "Hard stop should not wait for all commands";
}

/**
 * @test Проверка, что после команды soft stop поток завершается 
 * только после обработки всех команд
 */
TEST_F(ThreadCommandsTest, SoftStopCommandWaitsForAllTasks) {
    auto startCmd = std::make_shared<StartCommand>(executor);
    startCmd->execute();

    ASSERT_TRUE(executor->isRunning());

    // Добавляем команды
    std::atomic<int> counter{0};
    const int totalCommands = 10;
    
    for (int i = 0; i < totalCommands; i++) {
        queue->push(std::make_shared<DelayCommand>(
            std::chrono::milliseconds(10), counter));
    }

    // Мягкая остановка
    auto softStopCmd = std::make_shared<SoftStopCommand>(executor);
    softStopCmd->execute();

    // Поток все еще работает, обрабатывая оставшиеся команды
    // Ожидаем завершения
    executor->join();

    // Все команды должны быть выполнены
    ASSERT_EQ(totalCommands, counter.load()) 
        << "Soft stop should wait for all commands to complete";
    
    ASSERT_FALSE(executor->isRunning());
}

/**
 * @test Проверка разницы между hard и soft stop
 */
TEST_F(ThreadCommandsTest, HardVsSoftStopComparison) {
    // Test hard stop
    {
        auto queue1 = std::make_shared<LegacyThreadSafeQueue>();
        auto executor1 = std::make_shared<CommandExecutor>(queue1);
        
        executor1->start();
        
        std::atomic<int> counter1{0};
        for (int i = 0; i < 20; i++) {
            queue1->push(std::make_shared<DelayCommand>(
                std::chrono::milliseconds(20), counter1));
        }
        
        // Даем время на начало обработки
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        executor1->hardStop();
        int hardStopCount = counter1.load();
        
        ASSERT_LT(hardStopCount, 20) << "Hard stop should interrupt processing";
    }

    // Test soft stop
    {
        auto queue2 = std::make_shared<LegacyThreadSafeQueue>();
        auto executor2 = std::make_shared<CommandExecutor>(queue2);
        
        executor2->start();
        
        std::atomic<int> counter2{0};
        const int totalCommands = 20;
        
        for (int i = 0; i < totalCommands; i++) {
            queue2->push(std::make_shared<DelayCommand>(
                std::chrono::milliseconds(20), counter2));
        }
        
        executor2->softStop();
        executor2->join();
        
        int softStopCount = counter2.load();
        
        ASSERT_EQ(totalCommands, softStopCount) 
            << "Soft stop should complete all queued commands";
    }
}

/**
 * @test Проверка создания команд с nullptr
 */
TEST(ThreadCommandsCreationTest, ThrowsOnNullExecutor) {
    ASSERT_THROW(StartCommand(nullptr), std::invalid_argument);
    ASSERT_THROW(HardStopCommand(nullptr), std::invalid_argument);
    ASSERT_THROW(SoftStopCommand(nullptr), std::invalid_argument);
}