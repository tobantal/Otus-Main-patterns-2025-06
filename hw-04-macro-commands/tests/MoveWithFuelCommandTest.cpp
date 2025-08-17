#include <gtest/gtest.h>
#include "MoveWithFuelCommand.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тестовый набор для MoveWithFuelCommand
 */
class MoveWithFuelCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> spaceShip;

    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        spaceShip->setProperty("Position", Vector2D(0, 0));
        spaceShip->setProperty("Velocity", 10);
        spaceShip->setProperty("DirectionsNumber", 360);
        spaceShip->setProperty("Direction", 0);
        spaceShip->setProperty("FuelLevel", 100);
        spaceShip->setProperty("FuelConsumption", 10);
    }
};

/**
 * @test Успешное движение с расходом топлива
 */
TEST_F(MoveWithFuelCommandTest, ExecuteSuccessfully) {
    auto command = MoveWithFuelCommand::create(spaceShip);
    command->execute();

    // Проверяем изменение позиции
    Vector2D newPosition = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    ASSERT_EQ(Vector2D(10, 0), newPosition);

    // Проверяем расход топлива
    int newFuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(90, newFuelLevel);
}

/**
 * @test Исключение при недостаточном топливе
 */
TEST_F(MoveWithFuelCommandTest, ThrowsWhenInsufficientFuel) {
    spaceShip->setProperty("FuelLevel", 5);
    auto command = MoveWithFuelCommand::create(spaceShip);

    ASSERT_THROW(command->execute(), CommandException);

    // Проверяем, что позиция не изменилась
    Vector2D position = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    ASSERT_EQ(Vector2D(0, 0), position);

    // Проверяем, что топливо не было сожжено
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(5, fuelLevel);
}