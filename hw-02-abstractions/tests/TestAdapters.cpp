#include <gtest/gtest.h>
#include "MoveCommand.hpp"
#include "MovableAdapter.hpp"
#include "RotatableAdapter.hpp"
#include "SpaceShip.hpp"
#include "PhotonTorpedo.hpp"
#include <memory>
#include <cmath>

/**
 * @brief Тестовый класс для адаптеров
 */
class AdaptersTest : public ::testing::Test {
protected:
    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        torpedo = std::make_shared<PhotonTorpedo>();
    }

    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<PhotonTorpedo> torpedo;
};

/**
 * @brief Тест MovableAdapter с космическим кораблем
 */
TEST_F(AdaptersTest, MovableAdapterWithSpaceShip) {
    // Arrange
    spaceShip->setProperty("DirectionsNumber", 8); // 8 направлений
    spaceShip->setProperty("Position", Vector2D(10, 20));
    spaceShip->setProperty("Velocity", 5);         // Модуль скорости = 5
    spaceShip->setProperty("Direction", 2);        // Направление 2 = север (0, 1)

    MovableAdapter adapter(spaceShip);

    // Act & Assert - проверяем позицию
    Vector2D position = adapter.getPosition();
    EXPECT_EQ(position.x, 10);
    EXPECT_EQ(position.y, 20);

    // Проверяем скорость для направления "север" (direction = 2)
    // Направление 2 в 8-направленной системе = север = (0, 1)
    // vx = 5 * 0 = 0
    // vy = 5 * 1 = 5
    Vector2D velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, 0);
    EXPECT_EQ(velocity.y, 5);

    // Тест установки позиции
    adapter.setPosition(Vector2D(30, 40));
    Vector2D newPosition = adapter.getPosition();
    EXPECT_EQ(newPosition.x, 30);
    EXPECT_EQ(newPosition.y, 40);
}

/**
 * @brief Тест MovableAdapter с различными направлениями
 */
TEST_F(AdaptersTest, MovableAdapterWithDifferentDirections) {
    MovableAdapter adapter(spaceShip);

    spaceShip->setProperty("DirectionsNumber", 8); // 8 направлений

    // Тест направления 0 (восток)
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Velocity", 10);
    spaceShip->setProperty("Direction", 0);  // Восток (1, 0)

    Vector2D velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, 10);  // 10 * 1 = 10
    EXPECT_EQ(velocity.y, 0);   // 10 * 0 = 0

    // Тест направления 4 (запад)
    spaceShip->setProperty("Direction", 4);  // Запад (-1, 0)

    velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, -10); // 10 * (-1) = -10
    EXPECT_EQ(velocity.y, 0);   // 10 * 0 = 0

    // Тест направления 6 (юг)
    spaceShip->setProperty("Direction", 6);  // Юг (0, -1)

    velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, 0);   // 10 * 0 = 0
    EXPECT_EQ(velocity.y, -10); // 10 * (-1) = -10
}

/**
 * @brief Тест MovableAdapter с диагональными направлениями
 */
TEST_F(AdaptersTest, MovableAdapterWithDiagonalDirections) {
    MovableAdapter adapter(spaceShip);

    spaceShip->setProperty("DirectionsNumber", 8); // 8 направлений

    // Тест направления 1 (северо-восток)
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Velocity", 10);
    spaceShip->setProperty("Direction", 1);  // Северо-восток (1, 1) с нормализацией

    Vector2D velocity = adapter.getVelocity();
    // Диагональные направления нормализуются: 10 * 1 * 0.7 = 7
    EXPECT_EQ(velocity.x, 7);   // 10 * 1 * 0.7 = 7
    EXPECT_EQ(velocity.y, 7);   // 10 * 1 * 0.7 = 7

    // Тест направления 3 (северо-запад)
    spaceShip->setProperty("Direction", 3);  // Северо-запад (-1, 1) с нормализацией

    velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, -7);  // 10 * (-1) * 0.7 = -7
    EXPECT_EQ(velocity.y, 7);   // 10 * 1 * 0.7 = 7

    // Тест направления 5 (юго-запад)
    spaceShip->setProperty("Direction", 5);  // Юго-запад (-1, -1) с нормализацией

    velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, -7);  // 10 * (-1) * 0.7 = -7
    EXPECT_EQ(velocity.y, -7);  // 10 * (-1) * 0.7 = -7

    // Тест направления 7 (юго-восток)
    spaceShip->setProperty("Direction", 7);  // Юго-восток (1, -1) с нормализацией

    velocity = adapter.getVelocity();
    EXPECT_EQ(velocity.x, 7);   // 10 * 1 * 0.7 = 7
    EXPECT_EQ(velocity.y, -7);  // 10 * (-1) * 0.7 = -7
}

/**
 * @brief Тест MovableAdapter с торпедой
 */
TEST_F(AdaptersTest, MovableAdapterWithTorpedo) {
    // Arrange
    torpedo->setProperty("Position", Vector2D(5, 15));
    torpedo->setProperty("Velocity", Vector2D(3, 4));

    MovableAdapter adapter(torpedo);

    // Act & Assert
    Vector2D position = adapter.getPosition();
    EXPECT_EQ(position.x, 5);
    EXPECT_EQ(position.y, 15);

    // Для торпеды скорость берется напрямую как Vector2D, а не вычисляется
    // Но адаптер ожидает полярную систему, поэтому будет ошибка
    EXPECT_THROW(adapter.getVelocity(), std::runtime_error);
}

/**
 * @brief Альтернативный тест - создание торпеды совместимой с MovableAdapter
 */
TEST_F(AdaptersTest, TorpedoCompatibleWithMovableAdapter) {
    // Создаем торпеду с полярной системой координат для совместимости
    auto compatibleTorpedo = std::make_shared<PhotonTorpedo>();

    // Настраиваем торпеду в полярной системе
    compatibleTorpedo->setProperty("Position", Vector2D(0, 0));
    compatibleTorpedo->setProperty("Velocity", 15);      // Модуль скорости
    // Но торпеда не поддерживает Direction и DirectionsNumber...

    MovableAdapter adapter(compatibleTorpedo);

    // Позиция должна работать
    Vector2D position = adapter.getPosition();
    EXPECT_EQ(position.x, 0);
    EXPECT_EQ(position.y, 0);

    // Скорость не будет работать, так как торпеда не поддерживает Direction
    EXPECT_THROW(adapter.getVelocity(), std::runtime_error);
}

/**
 * @brief Тест RotatableAdapter
 */
TEST_F(AdaptersTest, RotatableAdapterWithSpaceShip) {
    // Arrange
    spaceShip->setProperty("DirectionsNumber", 360);
    spaceShip->setProperty("Direction", 180);
    spaceShip->setProperty("AngularVelocity", 45);

    RotatableAdapter adapter(spaceShip);

    // Act & Assert
    EXPECT_EQ(adapter.getDirection(), 180);
    EXPECT_EQ(adapter.getAngularVelocity(), 45);
    EXPECT_EQ(adapter.getDirectionsNumber(), 360);

    // Тест установки направления
    adapter.setDirection(270);
    EXPECT_EQ(adapter.getDirection(), 270);
}

/**
 * @brief Тест создания адаптеров с null-указателем
 */
TEST_F(AdaptersTest, NullGameObjectThrowsException) {
    EXPECT_THROW(MovableAdapter(nullptr), std::invalid_argument);
    EXPECT_THROW(RotatableAdapter(nullptr), std::invalid_argument);
}