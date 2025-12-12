#include <gtest/gtest.h>
#include "commands/StartMoveCommand.hpp"
#include "commands/StopMoveCommand.hpp"
#include "commands/FireCommand.hpp"
#include "UObject.hpp"
#include <memory>

/**
 * @file CommandsTest.cpp
 * @brief Тесты команд интерпретатора
 * @author Anton Tobolkin
 */

// ============================================================================
// Тесты StartMoveCommand
// ============================================================================

class StartMoveCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<UObject> createObject() {
        auto obj = std::make_shared<UObject>();
        obj->setProperty("id", std::string("test_ship"));
        return obj;
    }
    
    std::shared_ptr<UObject> createParams(int velocity) {
        auto params = std::make_shared<UObject>();
        params->setProperty("initialVelocity", velocity);
        return params;
    }
};

/**
 * @brief Тест успешного создания и выполнения StartMoveCommand
 */
TEST_F(StartMoveCommandTest, ExecuteSetsVelocity)
{
    auto object = createObject();
    auto params = createParams(10);
    
    StartMoveCommand cmd(object, params);
    cmd.execute();
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, 10);
}

/**
 * @brief Тест с нулевой скоростью
 */
TEST_F(StartMoveCommandTest, ExecuteWithZeroVelocity)
{
    auto object = createObject();
    auto params = createParams(0);
    
    StartMoveCommand cmd(object, params);
    cmd.execute();
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, 0);
}

/**
 * @brief Тест с отрицательной скоростью
 */
TEST_F(StartMoveCommandTest, ExecuteWithNegativeVelocity)
{
    auto object = createObject();
    auto params = createParams(-5);
    
    StartMoveCommand cmd(object, params);
    cmd.execute();
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, -5);
}

/**
 * @brief Тест ошибки при null объекте
 */
TEST_F(StartMoveCommandTest, NullObjectThrowsException)
{
    auto params = createParams(10);
    
    EXPECT_THROW(
        StartMoveCommand(nullptr, params),
        std::runtime_error
    );
}

/**
 * @brief Тест ошибки при null параметрах
 */
TEST_F(StartMoveCommandTest, NullParamsThrowsException)
{
    auto object = createObject();
    
    EXPECT_THROW(
        StartMoveCommand(object, nullptr),
        std::runtime_error
    );
}

/**
 * @brief Тест ошибки при отсутствии initialVelocity
 */
TEST_F(StartMoveCommandTest, MissingVelocityThrowsException)
{
    auto object = createObject();
    auto params = std::make_shared<UObject>();  // Без initialVelocity
    
    EXPECT_THROW(
        StartMoveCommand(object, params),
        std::runtime_error
    );
}

// ============================================================================
// Тесты StopMoveCommand
// ============================================================================

class StopMoveCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<UObject> createObject(int initialVelocity = 10) {
        auto obj = std::make_shared<UObject>();
        obj->setProperty("id", std::string("test_ship"));
        obj->setProperty("velocity", initialVelocity);
        return obj;
    }
};

/**
 * @brief Тест успешной остановки
 */
TEST_F(StopMoveCommandTest, ExecuteSetsVelocityToZero)
{
    auto object = createObject(100);
    auto params = std::make_shared<UObject>();
    
    StopMoveCommand cmd(object, params);
    cmd.execute();
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, 0);
}

/**
 * @brief Тест остановки уже остановленного объекта
 */
TEST_F(StopMoveCommandTest, ExecuteOnStoppedObject)
{
    auto object = createObject(0);
    auto params = std::make_shared<UObject>();
    
    StopMoveCommand cmd(object, params);
    EXPECT_NO_THROW(cmd.execute());
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, 0);
}

/**
 * @brief Тест работы без параметров (params = nullptr)
 */
TEST_F(StopMoveCommandTest, ExecuteWithNullParams)
{
    auto object = createObject(50);
    
    StopMoveCommand cmd(object, nullptr);
    cmd.execute();
    
    int velocity = std::any_cast<int>(object->getProperty("velocity"));
    EXPECT_EQ(velocity, 0);
}

/**
 * @brief Тест ошибки при null объекте
 */
TEST_F(StopMoveCommandTest, NullObjectThrowsException)
{
    EXPECT_THROW(
        StopMoveCommand(nullptr, nullptr),
        std::runtime_error
    );
}

// ============================================================================
// Тесты FireCommand
// ============================================================================

class FireCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<UObject> createObject() {
        auto obj = std::make_shared<UObject>();
        obj->setProperty("id", std::string("test_ship"));
        return obj;
    }
    
    std::shared_ptr<UObject> createParamsWithTarget(const std::string& targetId) {
        auto params = std::make_shared<UObject>();
        params->setProperty("targetId", targetId);
        return params;
    }
};

/**
 * @brief Тест выстрела с целью
 */
TEST_F(FireCommandTest, ExecuteWithTarget)
{
    auto object = createObject();
    auto params = createParamsWithTarget("enemy_001");
    
    FireCommand cmd(object, params);
    cmd.execute();
    
    std::string target = std::any_cast<std::string>(object->getProperty("lastFireTarget"));
    EXPECT_EQ(target, "enemy_001");
}

/**
 * @brief Тест выстрела без цели
 */
TEST_F(FireCommandTest, ExecuteWithoutTarget)
{
    auto object = createObject();
    auto params = std::make_shared<UObject>();  // Без targetId
    
    FireCommand cmd(object, params);
    cmd.execute();
    
    std::string target = std::any_cast<std::string>(object->getProperty("lastFireTarget"));
    EXPECT_EQ(target, "");  // Пустая строка
}

/**
 * @brief Тест выстрела с null параметрами
 */
TEST_F(FireCommandTest, ExecuteWithNullParams)
{
    auto object = createObject();
    
    FireCommand cmd(object, nullptr);
    cmd.execute();
    
    std::string target = std::any_cast<std::string>(object->getProperty("lastFireTarget"));
    EXPECT_EQ(target, "");
}

/**
 * @brief Тест ошибки при null объекте
 */
TEST_F(FireCommandTest, NullObjectThrowsException)
{
    auto params = createParamsWithTarget("enemy");
    
    EXPECT_THROW(
        FireCommand(nullptr, params),
        std::runtime_error
    );
}

/**
 * @brief Тест нескольких выстрелов подряд
 */
TEST_F(FireCommandTest, MultipleFiresUpdateTarget)
{
    auto object = createObject();
    
    auto params1 = createParamsWithTarget("enemy_001");
    FireCommand cmd1(object, params1);
    cmd1.execute();
    
    std::string target1 = std::any_cast<std::string>(object->getProperty("lastFireTarget"));
    EXPECT_EQ(target1, "enemy_001");
    
    auto params2 = createParamsWithTarget("enemy_002");
    FireCommand cmd2(object, params2);
    cmd2.execute();
    
    std::string target2 = std::any_cast<std::string>(object->getProperty("lastFireTarget"));
    EXPECT_EQ(target2, "enemy_002");
}