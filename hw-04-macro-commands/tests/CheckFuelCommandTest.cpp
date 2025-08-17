#include <gtest/gtest.h>
#include "CheckFuelCommand.hpp"
#include "FuelableAdapter.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тестовый набор для CheckFuelCommand
 */
class CheckFuelCommandTest : public ::testing::Test {
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
 * @test Успешная проверка при достаточном количестве топлива
 */
TEST_F(CheckFuelCommandTest, ExecuteWithSufficientFuel) {
    CheckFuelCommand command(fuelable);
    ASSERT_NO_THROW(command.execute());
}

/**
 * @test Проверка при точном совпадении уровня топлива и расхода
 */
TEST_F(CheckFuelCommandTest, ExecuteWithExactFuel) {
    spaceShip->setProperty("FuelLevel", 10);
    CheckFuelCommand command(fuelable);
    ASSERT_NO_THROW(command.execute());
}

/**
 * @test Исключение при недостаточном количестве топлива
 */
TEST_F(CheckFuelCommandTest, ThrowsWhenInsufficientFuel) {
    spaceShip->setProperty("FuelLevel", 5);
    CheckFuelCommand command(fuelable);
    ASSERT_THROW(command.execute(), CommandException);
}

/**
 * @test Исключение при нулевом уровне топлива
 */
TEST_F(CheckFuelCommandTest, ThrowsWhenNoFuel) {
    spaceShip->setProperty("FuelLevel", 0);
    CheckFuelCommand command(fuelable);
    ASSERT_THROW(command.execute(), CommandException);
}

/**
 * @test Исключение при попытке создать команду с nullptr
 */
TEST_F(CheckFuelCommandTest, ThrowsOnNullFuelable) {
    ASSERT_THROW(CheckFuelCommand(nullptr), std::invalid_argument);
}

/**
 * @test Исключение при отсутствии свойства FuelLevel
 */
TEST_F(CheckFuelCommandTest, ThrowsWhenFuelLevelMissing) {
    auto emptyShip = std::make_shared<SpaceShip>();
    emptyShip->setProperty("FuelConsumption", 10);
    auto fuelableEmpty = std::make_shared<FuelableAdapter>(emptyShip);
    CheckFuelCommand command(fuelableEmpty);
    ASSERT_THROW(command.execute(), std::runtime_error);
}