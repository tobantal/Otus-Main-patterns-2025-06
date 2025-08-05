#include <gtest/gtest.h>
#include "SpaceShip.hpp"
#include "PhotonTorpedo.hpp"
#include <memory>

/**
 * @brief �������� ����� ��� ������� ��������
 */
class GameObjectsTest : public ::testing::Test {
protected:
    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        torpedo = std::make_shared<PhotonTorpedo>(Vector2D(1, 2), Vector2D(3, 4));
    }

    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<PhotonTorpedo> torpedo;
};

/**
 * @brief ���� ������������� ������������ �������
 */
TEST_F(GameObjectsTest, SpaceShipInitialization) {
	// SpaceShip ����������� ���������� ��� ������� �������� �������� �� �������������
    EXPECT_THROW(spaceShip->getProperty("Position"), std::runtime_error);
    EXPECT_THROW(spaceShip->getProperty("Velocity"), std::runtime_error);
    EXPECT_THROW(spaceShip->getProperty("Direction"), std::runtime_error);
    EXPECT_THROW(spaceShip->getProperty("AngularVelocity"), std::runtime_error);
    EXPECT_THROW(spaceShip->getProperty("DirectionsNumber"), std::runtime_error);
}

/**
 * @brief ���� ��������� ������� ������������ �������
 */
TEST_F(GameObjectsTest, SpaceShipSetProperties) {
    // Acthip
	spaceShip->setProperty("DirectionsNumber", 360);
    spaceShip->setProperty("Position", Vector2D(10, 20));
    spaceShip->setProperty("Velocity", 15);
    spaceShip->setProperty("Direction", 90);
    spaceShip->setProperty("AngularVelocity", 5);

    // Assert
    auto position = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
    EXPECT_EQ(position.x, 10);
    EXPECT_EQ(position.y, 20);

    auto velocity = std::any_cast<int>(spaceShip->getProperty("Velocity"));
    EXPECT_EQ(velocity, 15);

    auto direction = std::any_cast<int>(spaceShip->getProperty("Direction"));
    EXPECT_EQ(direction, 90);

    auto angularVelocity = std::any_cast<int>(spaceShip->getProperty("AngularVelocity"));
    EXPECT_EQ(angularVelocity, 5);
}

/**
 * @brief ���� ��������� ��������������� ��������
 */
TEST_F(GameObjectsTest, SpaceShipNonExistentProperty) {
    // Act & Assert
    EXPECT_THROW(spaceShip->getProperty("NonExistent"), std::runtime_error);
}

/**
 * @brief ���� ������������� �������� �������
 */
TEST_F(GameObjectsTest, PhotonTorpedoInitialization) {
    // Act & Assert
    auto position = std::any_cast<Vector2D>(torpedo->getProperty("Position"));
    EXPECT_EQ(position.x, 1);
    EXPECT_EQ(position.y, 2);

    auto velocity = std::any_cast<Vector2D>(torpedo->getProperty("Velocity"));
    EXPECT_EQ(velocity.x, 3);
    EXPECT_EQ(velocity.y, 4);
}

/**
 * @brief ���� ����������� ������� �������
 */
TEST_F(GameObjectsTest, PhotonTorpedoPropertyRestrictions) {
    // Act & Assert - ������� ������������ ������ Position � Velocity
    EXPECT_THROW(torpedo->setProperty("Direction", 90), std::runtime_error);
    EXPECT_THROW(torpedo->getProperty("Direction"), std::runtime_error);
}

/**
 * @brief ���� �������� ������� � custom DirectionsNumber ����� ������������
 */
class CustomSpaceShip : public SpaceShip {
public:
    CustomSpaceShip(int directionsNumber) : SpaceShip() {
        // ���� ����� ������ �������� ������ �� ���������� ������������ �������� ������
        // � ���������� ��� ����� �������� ����� ��������� ����� �������������
    }
};