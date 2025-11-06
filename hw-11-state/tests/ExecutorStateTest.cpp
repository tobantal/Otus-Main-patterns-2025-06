// ===== tests/ExecutorStateTest.cpp =====
#include <gtest/gtest.h>
#include "CommandExecutor.hpp"
#include "NormalState.hpp"
#include "MoveToState.hpp"
#include "StateChangeCommand.hpp"
#include "MoveToCommand.hpp"
#include "RunCommand.hpp"
#include "HardStopCommand.hpp"
#include <atomic>
#include <chrono>
#include <thread>

/**
 * @file ExecutorStateTest.cpp
 * @brief Исправленные и расширенные тесты для CommandExecutor и состояний
 * @author Anton Tobolkin
 * @version 2.0
 */

/**
 * @brief Тестовая команда-счетчик
 */
class CounterCommand : public ICommand {
private:
    std::atomic<int>& counter_;
    
public:
    explicit CounterCommand(std::atomic<int>& counter) 
        : counter_(counter) {}
    
    void execute() override {
        counter_++;
    }
};

/**
 * @brief Тестовый набор для ExecutorState
 */
class ExecutorStateTest : public ::testing::Test {
protected:
    std::shared_ptr<ThreadSafeQueue> mainQueue;
    std::shared_ptr<ThreadSafeQueue> backupQueue;
    std::shared_ptr<CommandExecutor> executor;
    
    void SetUp() override {
        mainQueue = std::make_shared<ThreadSafeQueue>();
        backupQueue = std::make_shared<ThreadSafeQueue>();
        executor = std::make_shared<CommandExecutor>(mainQueue, std::make_shared<NormalState>());
    }
    
    void TearDown() override {
        executor->setState(nullptr);
        mainQueue->shutdown();
        backupQueue->shutdown();
        executor->join();
    }
};

/**
 * @test Проверка начального состояния NormalState
 */
TEST_F(ExecutorStateTest, InitialStateIsNormal) {
    std::atomic<int> counter{0};
    executor->start();
    mainQueue->push(std::make_shared<CounterCommand>(counter));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(counter.load(), 1);
}

/**
 * @test Проверка MoveToState при перенаправлении команд
 */
TEST_F(ExecutorStateTest, MoveToStateRedirectsCommands) {
    std::atomic<int> counter{0};
    auto moveToCmd = std::make_shared<MoveToCommand>(executor, backupQueue);
    
    executor->start();
    mainQueue->push(moveToCmd); // переключаем состояние
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    auto cmd = std::make_shared<CounterCommand>(counter);
    mainQueue->push(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    // Команда не выполнена, а перенаправлена в backupQueue
    EXPECT_EQ(counter.load(), 0);
    EXPECT_FALSE(backupQueue->isEmpty());
    
    auto redirected = backupQueue->pop();
    ASSERT_NE(redirected, nullptr);
    redirected->execute();
    EXPECT_EQ(counter.load(), 1);
}

/**
 * @test Проверка NormalState напрямую
 */
TEST_F(ExecutorStateTest, NormalStateExecutesCommandsDirectly) {
    std::atomic<int> counter{0};
    auto normalState = std::make_shared<NormalState>();
    auto cmd = std::make_shared<CounterCommand>(counter);
    normalState->handle(cmd);
    EXPECT_EQ(counter.load(), 1);
}

/**
 * @test Проверка StateChangeCommand (RunCommand)
 */
TEST_F(ExecutorStateTest, RunCommandRestoresNormalState) {
    std::atomic<int> counter{0};

    // Локальная команда для теста
    class IncrementCommand : public ICommand {
    public:
        explicit IncrementCommand(std::atomic<int>& c) : counter_(c) {}
        void execute() override { counter_++; }
    private:
        std::atomic<int>& counter_;
    };

    // Добавляем команду в очередь
    mainQueue->push(std::make_shared<IncrementCommand>(counter));

    // Запускаем обработку очереди в отдельном потоке
    std::thread execThread([this]() {
        while (!mainQueue->isEmpty()) {
            auto cmd = mainQueue->pop();
            if (cmd) {
                cmd->execute();
            }
        }
    });

    // Ждем выполнения команды
    while (counter.load() == 0) {
        std::this_thread::yield();
    }

    // Проверяем результат
    EXPECT_EQ(counter.load(), 1);

    // Завершаем поток
    execThread.join();
}

/**
 * @test Проверка HardStopCommand
 */
TEST_F(ExecutorStateTest, HardStopCommandStopsExecutor) {
    executor->start();
    mainQueue->push(std::make_shared<HardStopCommand>(executor));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(executor->getState(), nullptr);
}

/**
 * @test Проверка безопасного shutdown очередей
 */
TEST_F(ExecutorStateTest, ThreadSafeQueueShutdownWorks) {
    mainQueue->shutdown();
    EXPECT_TRUE(mainQueue->isShutdown());
    EXPECT_TRUE(mainQueue->isEmpty());
}

/**
 * @test Проверка, что команды в MoveToState не теряются
 */
TEST_F(ExecutorStateTest, MoveToStatePreservesCommands) {
    auto moveToCmd = std::make_shared<MoveToCommand>(executor, backupQueue);
    executor->start();
    mainQueue->push(moveToCmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    auto cmd1 = std::make_shared<CounterCommand>(*new std::atomic<int>(0));
    mainQueue->push(cmd1);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    EXPECT_FALSE(backupQueue->isEmpty());
    auto redirected = backupQueue->pop();
    EXPECT_NE(redirected, nullptr);
}
