#include <gtest/gtest.h>
#include "ChangeVelocityCommand.hpp"
#include "VelocityChangeableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тестовый набор для ChangeVelocityCommand
 */
class ChangeVelocityCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<IVelocityChangeable> velocityChangeable;

    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        spaceShip->setProperty("Velocity", 10);
        spaceShip->setProperty("DirectionsNumber", 360);
        spaceShip->setProperty("Direction", 45);
        velocityChangeable = std::make_shared<VelocityChangeableAdapter>(spaceShip);
    }
};

/**
 * @test Успешное выполнение при движущемся объекте
 */
TEST_F(ChangeVelocityCommandTest, ExecuteWithMovingObject) {
    ChangeVelocityCommand command(velocityChangeable);
    ASSERT_NO_THROW(command.execute());
}

/**
 * @test Ничего не делает при неподвижном объекте
 */
TEST_F(ChangeVelocityCommandTest, DoesNothingWhenVelocityIsZero) {
    spaceShip->setProperty("Velocity", 0);
    ChangeVelocityCommand command(velocityChangeable);
    ASSERT_NO_THROW(command.execute());
}

/**
 * @test Исключение при попытке создать команду с nullptr
 */
TEST_F(ChangeVelocityCommandTest, ThrowsOnNullVelocityChangeable) {
    ASSERT_THROW(ChangeVelocityCommand(nullptr), std::invalid_argument);
}