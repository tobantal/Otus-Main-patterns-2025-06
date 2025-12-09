#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CheckCollisionCommand.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Mock для ICollisionChecker
 */
class MockChecker : public ICollisionChecker {
public:
    MOCK_METHOD(bool, checkCollision, 
                (const IGameObject&, const IGameObject&), (const, override));
};

/**
 * @brief Тесты для CheckCollisionCommand
 */
class CheckCollisionCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<MockChecker> checker;
    std::shared_ptr<SpaceShip> obj1;
    std::shared_ptr<SpaceShip> obj2;
    
    void SetUp() override {
        checker = std::make_shared<MockChecker>();
        obj1 = std::make_shared<SpaceShip>();
        obj2 = std::make_shared<SpaceShip>();
        
        obj1->setProperty("Position", Vector2D(0, 0));
        obj2->setProperty("Position", Vector2D(10, 10));
    }
};

/**
 * @test Команда вызывает checker
 */
TEST_F(CheckCollisionCommandTest, CallsChecker) {
    EXPECT_CALL(*checker, checkCollision(testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Return(false));
    
    CheckCollisionCommand cmd(obj1, obj2, checker);
    EXPECT_NO_THROW(cmd.execute());
}

/**
 * @test Получение объектов
 */
TEST_F(CheckCollisionCommandTest, GetObjects) {
    CheckCollisionCommand cmd(obj1, obj2, checker);
    
    EXPECT_EQ(obj1.get(), cmd.getObject1().get());
    EXPECT_EQ(obj2.get(), cmd.getObject2().get());
}

/**
 * @test Исключение при null object1
 */
TEST_F(CheckCollisionCommandTest, ThrowsOnNullObject1) {
    EXPECT_THROW(
        CheckCollisionCommand(nullptr, obj2, checker),
        std::invalid_argument
    );
}

/**
 * @test Исключение при null object2
 */
TEST_F(CheckCollisionCommandTest, ThrowsOnNullObject2) {
    EXPECT_THROW(
        CheckCollisionCommand(obj1, nullptr, checker),
        std::invalid_argument
    );
}

/**
 * @test Исключение при null checker
 */
TEST_F(CheckCollisionCommandTest, ThrowsOnNullChecker) {
    EXPECT_THROW(
        CheckCollisionCommand(obj1, obj2, nullptr),
        std::invalid_argument
    );
}

/**
 * @test Многократное выполнение команды
 */
TEST_F(CheckCollisionCommandTest, MultipleExecutions) {
    EXPECT_CALL(*checker, checkCollision(testing::_, testing::_))
        .Times(3)
        .WillRepeatedly(testing::Return(true));
    
    CheckCollisionCommand cmd(obj1, obj2, checker);
    
    EXPECT_NO_THROW(cmd.execute());
    EXPECT_NO_THROW(cmd.execute());
    EXPECT_NO_THROW(cmd.execute());
}