// ===== tests/StateCommandsTest.cpp =====
#include <gtest/gtest.h>
#include "MoveToCommand.hpp"
#include "RunCommand.hpp"
#include "HardStopCommand.hpp"
#include "CommandExecutor.hpp"
#include "ThreadSafeQueue.hpp"
#include "NormalState.hpp"
#include "MoveToState.hpp"
#include <atomic>
#include <thread>
#include <chrono>

/**
 * @file StateCommandsTest.cpp
 * @brief Тесты команд смены состояния (MoveTo, Run, HardStop)
 */

class StateCommandsTest : public ::testing::Test {
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

    // Простая команда для проверки перенаправления
    class CounterCommand : public ICommand {
    public:
        explicit CounterCommand(std::atomic<int>& counter) : counter(counter) {}
        void execute() override { counter.fetch_add(1); }
    private:
        std::atomic<int>& counter;
    };
};

TEST_F(StateCommandsTest, MoveToCommandSetsMoveToState) {
    executor->start();
    auto moveToCmd = std::make_shared<MoveToCommand>(executor, backupQueue);
    moveToCmd->execute();

    auto state = executor->getState();
    ASSERT_NE(state, nullptr);

    // Проверяем, что state — это MoveToState
    auto moveToState = std::dynamic_pointer_cast<MoveToState>(state);
    ASSERT_NE(moveToState, nullptr);

    // Проверяем, что targetQueue у MoveToState совпадает с backupQueue
    EXPECT_EQ(moveToState->getTargetQueue(), backupQueue);
}

TEST_F(StateCommandsTest, RunCommandSetsNormalState) {
    executor->start();
    auto runCmd = std::make_shared<RunCommand>(executor);
    runCmd->execute();

    auto state = executor->getState();
    ASSERT_NE(state, nullptr);

    auto normalState = std::dynamic_pointer_cast<NormalState>(state);
    EXPECT_NE(normalState, nullptr);
}

TEST_F(StateCommandsTest, HardStopCommandSetsNullptr) {
    executor->start();
    auto stopCmd = std::make_shared<HardStopCommand>(executor);
    stopCmd->execute();
    EXPECT_EQ(executor->getState(), nullptr);
}

TEST_F(StateCommandsTest, MoveToCommandWithNullQueueThrows) {
    EXPECT_THROW(MoveToCommand(executor, nullptr), std::invalid_argument);
}

TEST_F(StateCommandsTest, RunCommandWithNullExecutorThrows) {
    EXPECT_THROW(RunCommand(nullptr), std::invalid_argument);
}

TEST_F(StateCommandsTest, HardStopCommandWithNullExecutorThrows) {
    EXPECT_THROW(HardStopCommand(nullptr), std::invalid_argument);
}

TEST_F(StateCommandsTest, MoveToCommandRedirectsCommands) {
    std::atomic<int> counter{0};
    executor->start();

    auto moveToCmd = std::make_shared<MoveToCommand>(executor, backupQueue);
    moveToCmd->execute();
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
