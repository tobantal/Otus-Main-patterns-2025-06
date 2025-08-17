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
 * @brief �������� ����� ��� MacroCommand
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
 * @test �������� ���������� ������������������ ������
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
 * @test ���������� ���������� ��� ���������� CommandException
 */
TEST_F(MacroCommandTest, StopsOnCommandException) {
    spaceShip->setProperty("FuelLevel", 5); // ������������ �������

    auto movable = std::make_shared<MovableAdapter>(spaceShip);
    auto fuelable = std::make_shared<FuelableAdapter>(spaceShip);

    std::vector<std::shared_ptr<ICommand>> commands = {
        std::make_shared<CheckFuelCommand>(fuelable),  // �������� ����������
        std::make_shared<MoveCommand>(movable),         // �� ������ �����������
        std::make_shared<BurnFuelCommand>(fuelable)     // �� ������ �����������
    };

    MacroCommand macro(commands);
    ASSERT_THROW(macro.execute(), CommandException);

    // ���������, ��� ������� �� ����������
    Vector2D position = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    ASSERT_EQ(Vector2D(0, 0), position);

    // ���������, ��� ������� �� ���� �������
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(5, fuelLevel);
}

/**
 * @test ���������� ��� ������ ������� ������
 */
TEST_F(MacroCommandTest, ThrowsOnEmptyCommands) {
    std::vector<std::shared_ptr<ICommand>> emptyCommands;
    ASSERT_THROW({
    MacroCommand macro(emptyCommands);
        }, std::invalid_argument);
}

/**
 * @test ���������� ��� ������� nullptr � ������� ������
 */
TEST_F(MacroCommandTest, ThrowsOnNullCommand) {
    auto movable = std::make_shared<MovableAdapter>(spaceShip);

    std::vector<std::shared_ptr<ICommand>> commands = {
        std::make_shared<MoveCommand>(movable),
        nullptr,  // �����������
        std::make_shared<MoveCommand>(movable)
    };

    ASSERT_THROW({
    MacroCommand macro(commands);
        }, std::invalid_argument);
}