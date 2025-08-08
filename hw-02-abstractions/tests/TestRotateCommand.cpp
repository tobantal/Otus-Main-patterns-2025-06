#include <gtest/gtest.h>
#include "RotateCommand.hpp"
#include "SpaceShip.hpp"
#include "RotatableAdapter.hpp"
#include <memory>

/**
 * @brief �������� ����� ��� RotateCommand
 */
class RotateCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        rotatableAdapter = std::make_shared<RotatableAdapter>(spaceShip);
    }

    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<RotatableAdapter> rotatableAdapter;
};

/**
 * @brief ���� ��������� �������� �������
 */
TEST_F(RotateCommandTest, SuccessfulRotation) {
    // Arrange
    spaceShip->setProperty("DirectionsNumber", 360);
    spaceShip->setProperty("Direction", 45);
    spaceShip->setProperty("AngularVelocity", 90);

    RotateCommand rotateCommand(rotatableAdapter);

    // Act
    rotateCommand.execute();

    // Assert
    int newDirection = rotatableAdapter->getDirection();
    EXPECT_EQ(newDirection, 135);
}

/**
 * @brief ���� �������� � ������������� (����������� �������)
 */
TEST_F(RotateCommandTest, RotationWithOverflow) {
    // Arrange
    spaceShip->setProperty("DirectionsNumber", 360);
    spaceShip->setProperty("Direction", 350);
    spaceShip->setProperty("AngularVelocity", 30);

    RotateCommand rotateCommand(rotatableAdapter);

    // Act
    rotateCommand.execute();

    // Assert
    int newDirection = rotatableAdapter->getDirection();
    EXPECT_EQ(newDirection, 20); // (350 + 30) % 360 = 20
}

/**
 * @brief ���� �������� � ������������� ������� ���������
 */
TEST_F(RotateCommandTest, RotationWithNegativeAngularVelocity) {
    // Arrange
    spaceShip->setProperty("DirectionsNumber", 360);
    spaceShip->setProperty("Direction", 30);
    spaceShip->setProperty("AngularVelocity", -50);

    RotateCommand rotateCommand(rotatableAdapter);

    // Act
    rotateCommand.execute();

    // Assert
    int newDirection = rotatableAdapter->getDirection();
    EXPECT_EQ(newDirection, 340); // (30 - 50 + 360) % 360 = 340
}

/**
 * @brief ���-����� ��� ������������ ������ ������ �����������
 */
class MockRotatableNoDirection : public IRotatable {
public:
    int getDirection() const override {
        throw std::runtime_error("Cannot read direction");
    }

    void setDirection(int direction) override {
        // �� ������������ � ���� �����
    }

    int getAngularVelocity() const override {
        return 10;
    }

    int getDirectionsNumber() const override {
        return 360;
    }
};

/**
 * @brief ���� ������ ��� ������������� ��������� �����������
 */
TEST_F(RotateCommandTest, CannotReadDirection) {
    // Arrange
    auto mockRotatable = std::make_shared<MockRotatableNoDirection>();
    RotateCommand rotateCommand(mockRotatable);

    // Act & Assert
    EXPECT_THROW(rotateCommand.execute(), std::runtime_error);
}

/**
 * @brief ���� �������� ������� � null-����������
 */
TEST_F(RotateCommandTest, NullRotatableThrowsException) {
    // Act & Assert
    EXPECT_THROW(RotateCommand(nullptr), std::invalid_argument);
}