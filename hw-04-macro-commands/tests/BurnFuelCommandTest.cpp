#include <gtest/gtest.h>
#include "BurnFuelCommand.hpp"
#include "FuelableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief �������� ����� ��� BurnFuelCommand
 */
class BurnFuelCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<IFuelable> fuelable;

    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        spaceShip->setProperty("FuelLevel", 100);
        spaceShip->setProperty("FuelConsumption", 10);
        fuelable = std::make_shared<FuelableAdapter>(spaceShip);
    }
};

/**
 * @test �������� �������� �������
 */
TEST_F(BurnFuelCommandTest, ExecuteSuccessfully) {
    BurnFuelCommand command(fuelable);
    command.execute();

    int newFuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(90, newFuelLevel);
}

/**
 * @test ������������ �������� �������
 */
TEST_F(BurnFuelCommandTest, ExecuteMultipleTimes) {
    BurnFuelCommand command(fuelable);

    command.execute();
    command.execute();
    command.execute();

    int newFuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(70, newFuelLevel);
}

/**
 * @test �������� ������� �� �������������� ��������
 */
TEST_F(BurnFuelCommandTest, BurnToNegativeFuel) {
    spaceShip->setProperty("FuelLevel", 5);
    BurnFuelCommand command(fuelable);

    command.execute();

    int newFuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(-5, newFuelLevel);
}

/**
 * @test �������� ��� ������� �������
 */
TEST_F(BurnFuelCommandTest, BurnWithZeroConsumption) {
    spaceShip->setProperty("FuelConsumption", 0);
    BurnFuelCommand command(fuelable);

    command.execute();

    int newFuelLevel = std::any_cast<int>(spaceShip->getProperty("FuelLevel"));
    ASSERT_EQ(100, newFuelLevel);
}

/**
 * @test ���������� ��� ������� ������� ������� � nullptr
 */
TEST_F(BurnFuelCommandTest, ThrowsOnNullFuelable) {
    ASSERT_THROW(BurnFuelCommand(nullptr), std::invalid_argument);
}

/**
 * @test ���������� ��� ���������� �������� FuelLevel
 */
TEST_F(BurnFuelCommandTest, ThrowsWhenFuelLevelMissing) {
    auto emptyShip = std::make_shared<SpaceShip>();
    emptyShip->setProperty("FuelConsumption", 10);
    auto fuelableEmpty = std::make_shared<FuelableAdapter>(emptyShip);
    BurnFuelCommand command(fuelableEmpty);
    ASSERT_THROW(command.execute(), std::runtime_error);
}

/**
 * @test ���������� ��� ���������� �������� FuelConsumption
 */
TEST_F(BurnFuelCommandTest, ThrowsWhenFuelConsumptionMissing) {
    auto emptyShip = std::make_shared<SpaceShip>();
    emptyShip->setProperty("FuelLevel", 100);
    auto fuelableEmpty = std::make_shared<FuelableAdapter>(emptyShip);
    BurnFuelCommand command(fuelableEmpty);
    ASSERT_THROW(command.execute(), std::runtime_error);
}