#include <gtest/gtest.h>
#include "RotateWithVelocityChangeCommand.hpp"
#include "MovableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief �������� ����� ��� RotateWithVelocityChangeCommand
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
 * @test ������� ����������� ������� ��� �������
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateMovingObjectWithoutFuel) {
    auto command = RotateWithVelocityChangeCommand::create(spaceShip, false);
    command->execute();

    // ��������� ��������� �����������
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);

    // ������� �� ������ ����������
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(100, fuelLevel);
}

/**
 * @test ������� ����������� ������� � ��������
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateMovingObjectWithFuel) {
    auto command = RotateWithVelocityChangeCommand::create(spaceShip, true);
    command->execute();

    // ��������� ��������� �����������
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);

    // ��������� ������ �������
    int fuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(95, fuelLevel);
}

/**
 * @test ������� ������������ �������
 */
TEST_F(RotateWithVelocityChangeCommandTest, RotateStationaryObject) {
    spaceShip->setProperty("Velocity", 0); // ������ �� ��������

    auto command = RotateWithVelocityChangeCommand::create(spaceShip, false);
    command->execute();

    // ��������� ��������� �����������
    int newDirection = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(45, newDirection);
}

/**
 * @test ���������� ��� ������������� �������
 */
TEST_F(RotateWithVelocityChangeCommandTest, ThrowsWhenInsufficientFuel) {
    spaceShip->setProperty("FuelLevel", 2);

    auto command = RotateWithVelocityChangeCommand::create(spaceShip, true);
    ASSERT_THROW(command->execute(), CommandException);

    // ���������, ��� ����������� �� ����������
    int direction = std::any_cast<int>(spaceShip->getProperty("Direction"));
    ASSERT_EQ(0, direction);
}