#include <gtest/gtest.h>
#include "MacroCommand.hpp"
#include "MoveCommand.hpp"
#include "RotateCommand.hpp"
#include "CheckFuelCommand.hpp"
#include "BurnFuelCommand.hpp"
#include "MovableAdapter.hpp"
#include "RotatableAdapter.hpp"
#include "FuelableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тестовый набор для MacroCommand
 */
class MacroCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> spaceShip;

    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        spaceShip->setProperty("Position", Vector2D(0, 0));
        spaceShip->setProperty("Velocity", 10);
        spaceShip->setProperty("DirectionsNumber", 360);
        spaceShip->setProperty("Direction", 0);
        spaceShip->setProperty("AngularVelocity", 45);
        spaceShip->setProperty("FuelLevel", 100);
        spaceShip->setProperty("FuelConsumption", 10);
    }
};

/**
 * @test Успешное выполнение последовательности команд
 */
TEST_F(MacroCommandTest, ExecuteSequenceSuccessfully) {
    auto movable = std::make_shared<MovableAdapter>(spaceShip);
    auto rotatable = std::make_shared<RotatableAdapter>(spaceShip);

    std::vector<std::shared_ptr<ICommand>> commands = {
        std::make_shared<MoveCommand>(movable),
        std::make_shared<RotateCommand>(rotatable)
    };

    MacroCommand macro(commands);
    ASSERT_NO_THROW(macro.execute());

    Vector2D newPosition = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));

    ASSERT_EQ(Vector2D(10, 0), newPosition);
    ASSERT_EQ(45, newDirection);
}

/**
 * @test Прерывание выполнения при исключении CommandException
 */
TEST_F(MacroCommandTest, StopsOnCommandException) {
    spaceShip->setProperty("FuelLevel", 5); // Недостаточно топлива

    auto movable = std::make_shared<MovableAdapter>(spaceShip);
    auto fuelable = std::make_shared<FuelableAdapter>(spaceShip);

    std::vector<std::shared_ptr<ICommand>> commands = {
        std::make_shared<CheckFuelCommand>(fuelable),  // Выбросит исключение
        std::make_shared<MoveCommand>(movable),         // Не должно выполниться
        std::make_shared<BurnFuelCommand>(fuelable)     // Не должно выполниться
    };

    MacroCommand macro(commands);
    ASSERT_THROW(macro.execute(), CommandException);

    // Проверяем, что позиция не изменилась
    Vector2D position = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    ASSERT_EQ(Vector2D(0, 0), position);

    // Проверяем, что топливо не было сожжено
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(5, fuelLevel);
}

/**
 * @test Исключение при пустом векторе команд
 */
TEST_F(MacroCommandTest, ThrowsOnEmptyCommands) {
    std::vector<std::shared_ptr<ICommand>> emptyCommands;
    ASSERT_THROW({
    MacroCommand macro(emptyCommands);
        }, std::invalid_argument);
}

/**
 * @test Исключение при наличии nullptr в векторе команд
 */
TEST_F(MacroCommandTest, ThrowsOnNullCommand) {
    auto movable = std::make_shared<MovableAdapter>(spaceShip);

    std::vector<std::shared_ptr<ICommand>> commands = {
        std::make_shared<MoveCommand>(movable),
        nullptr,  // Недопустимо
        std::make_shared<MoveCommand>(movable)
    };

    ASSERT_THROW({
    MacroCommand macro(commands);
        }, std::invalid_argument);
}