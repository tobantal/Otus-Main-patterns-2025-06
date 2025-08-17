#include <gtest/gtest.h>
#include "RotateWithVelocityChangeCommand.hpp"
#include "MovableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тестовый набор для RotateWithVelocityChangeCommand
 */
class RotateWithVelocityChangeCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> spaceShip;

    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        spaceShip->setProperty("DirectionsNumber", 360);
        spaceShip->setProperty("Direction", 0);
        spaceShip->setProperty("AngularVelocity", 45);
        spaceShip->setProperty("Velocity", 10);
        spaceShip->setProperty("FuelLevel", 100);
        spaceShip->setProperty("FuelConsumption", 5);
    }
};

/**
 * @test Поворот движущегося объекта без топлива
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateMovingObjectWithoutFuel) {
    auto command = RotateWithVelocityChangeCommand::create(spaceShip, false);
    command->execute();

    // Проверяем изменение направления
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);

    // Топливо не должно измениться
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(100, fuelLevel);
}

/**
 * @test Поворот движущегося объекта с топливом
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateMovingObjectWithFuel) {
    auto command = RotateWithVelocityChangeCommand::create(spaceShip, true);
    command->execute();

    // Проверяем изменение направления
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);

    // Проверяем расход топлива
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(95, fuelLevel);
}

/**
 * @test Поворот неподвижного объекта
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateStationaryObject) {
    spaceShip->setProperty("Velocity", 0); // Объект не движется

    auto command = RotateWithVelocityChangeCommand::create(spaceShip, false);
    command->execute();

    // Проверяем изменение направления
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);
}

/**
 * @test Исключение при недостаточном топливе
 */
TEST_F(RotateWithVelocityChangeCommandTest, ThrowsWhenInsufficientFuel) {
    spaceShip->setProperty("FuelLevel", 2);

    auto command = RotateWithVelocityChangeCommand::create(spaceShip, true);
    ASSERT_THROW(command->execute(), CommandException);

    // Проверяем, что направление не изменилось
    int direction = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(0, direction);
}