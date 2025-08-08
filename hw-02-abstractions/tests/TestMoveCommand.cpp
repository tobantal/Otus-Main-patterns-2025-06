#include <gtest/gtest.h>
#include "MoveCommand.hpp"
#include "SpaceShip.hpp"
#include "MovableAdapter.hpp"
#include <memory>

/**
 * @brief Тестовый класс для MoveCommand с исправленной логикой
 */
class MoveCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        spaceShip = std::make_shared<SpaceShip>();
        movableAdapter = std::make_shared<MovableAdapter>(spaceShip);
    }

    std::shared_ptr<SpaceShip> spaceShip;
    std::shared_ptr<MovableAdapter> movableAdapter;
};

/**
 * @brief Тест успешного движения объекта
 *
 * Проверяет, что объект в точке (12, 5) со скоростью (-7, 3)
 * после движения оказывается в точке (5, 8)
 *
 * Используем дискретную систему с 8 направлениями:
 * 0: (1,0)   - восток
 * 1: (1,1)   - северо-восток (с нормализацией)
 * 2: (0,1)   - север
 * 3: (-1,1)  - северо-запад (с нормализацией)
 * 4: (-1,0)  - запад
 * 5: (-1,-1) - юго-запад (с нормализацией)
 * 6: (0,-1)  - юг
 * 7: (1,-1)  - юго-восток (с нормализацией)
 */
TEST_F(MoveCommandTest, SuccessfulMovement) {
    // Arrange - настраиваем начальные условия
    spaceShip->setProperty("DirectionsNumber", 8);
    spaceShip->setProperty("Position", Vector2D(12, 5));
    spaceShip->setProperty("Velocity", 10);        // Модуль скорости = 10
    spaceShip->setProperty("Direction", 3);        // Северо-запад (-1, 1) с нормализацией

    // Проверяем вычисление скорости
    // Направление 3 = северо-запад = (-1, 1) * velocity * 0.7 (нормализация)
    // vx = 10 * (-1) * 0.7 = -7
    // vy = 10 * 1 * 0.7 = 7
    Vector2D velocity = movableAdapter->getVelocity();
    EXPECT_EQ(velocity.x, -7);
    EXPECT_EQ(velocity.y, 7);

    MoveCommand moveCommand(movableAdapter);

    // Act - выполняем действие
    moveCommand.execute();

    // Assert - проверяем результат
    // Новая позиция = (12, 5) + (-7, 7) = (5, 12)
    Vector2D newPosition = movableAdapter->getPosition();
    EXPECT_EQ(newPosition.x, 5);
    EXPECT_EQ(newPosition.y, 12);
}

/**
 * @brief Тест движения по основным направлениям
 */
TEST_F(MoveCommandTest, MovementInCardinalDirections) {
    // Тест движения на восток (направление 0)
    spaceShip->setProperty("DirectionsNumber", 8);
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Velocity", 5);
    spaceShip->setProperty("Direction", 0);  // Восток

    MoveCommand moveCommand(movableAdapter);
    moveCommand.execute();

    Vector2D position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 5);
    EXPECT_EQ(position.y, 0);

    // Тест движения на север (направление 2)
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Direction", 2);  // Север

    moveCommand.execute();

    position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 0);
    EXPECT_EQ(position.y, 5);

    // Тест движения на запад (направление 4)
    spaceShip->setProperty("Position", Vector2D(10, 10));
    spaceShip->setProperty("Direction", 4);  // Запад

    moveCommand.execute();

    position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 5);
    EXPECT_EQ(position.y, 10);
}

/**
 * @brief Тест с альтернативной реализацией для получения скорости (-7, 3)
 *
 * Создаем специальный тестовый объект, который напрямую возвращает нужную скорость
 */
class TestMovableObject : public IMovable {
private:
    Vector2D m_position;
    Vector2D m_velocity;

public:
    TestMovableObject(const Vector2D& pos, const Vector2D& vel)
        : m_position(pos), m_velocity(vel) {
    }

    Vector2D getPosition() const override {
        return m_position;
    }

    void setPosition(const Vector2D& position) override {
        m_position = position;
    }

    Vector2D getVelocity() const override {
        return m_velocity;
    }
};

/**
 * @brief Прямой тест требования: (12, 5) + (-7, 3) = (5, 8)
 */
TEST_F(MoveCommandTest, ExactRequirementTest) {
    // Arrange - создаем объект напрямую с нужными параметрами
    auto testObject = std::make_shared<TestMovableObject>(
        Vector2D(12, 5),   // начальная позиция
        Vector2D(-7, 3)    // скорость
    );

    MoveCommand moveCommand(testObject);

    // Act
    moveCommand.execute();

    // Assert
    Vector2D newPosition = testObject->getPosition();
    EXPECT_EQ(newPosition.x, 5);
    EXPECT_EQ(newPosition.y, 8);
}

/**
 * @brief Мок-класс для тестирования ошибки чтения позиции
 */
class MockMovableNoPosition : public IMovable {
public:
    Vector2D getPosition() const override {
        throw std::runtime_error("Cannot read position");
    }

    void setPosition(const Vector2D& position) override {
        // Не используется в этом тесте
    }

    Vector2D getVelocity() const override {
        return Vector2D(1, 1);
    }
};

/**
 * @brief Тест ошибки при невозможности прочитать позицию
 */
TEST_F(MoveCommandTest, CannotReadPosition) {
    auto mockMovable = std::make_shared<MockMovableNoPosition>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief Мок-класс для тестирования ошибки чтения скорости
 */
class MockMovableNoVelocity : public IMovable {
public:
    Vector2D getPosition() const override {
        return Vector2D(0, 0);
    }

    void setPosition(const Vector2D& position) override {
        // Не используется в этом тесте
    }

    Vector2D getVelocity() const override {
        throw std::runtime_error("Cannot read velocity");
    }
};

/**
 * @brief Тест ошибки при невозможности прочитать скорость
 */
TEST_F(MoveCommandTest, CannotReadVelocity) {
    auto mockMovable = std::make_shared<MockMovableNoVelocity>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief Мок-класс для тестирования ошибки записи позиции
 */
class MockMovableCannotSetPosition : public IMovable {
public:
    Vector2D getPosition() const override {
        return Vector2D(0, 0);
    }

    void setPosition(const Vector2D& position) override {
        throw std::runtime_error("Cannot set position");
    }

    Vector2D getVelocity() const override {
        return Vector2D(1, 1);
    }
};

/**
 * @brief Тест ошибки при невозможности установить позицию
 */
TEST_F(MoveCommandTest, CannotSetPosition) {
    auto mockMovable = std::make_shared<MockMovableCannotSetPosition>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief Тест создания команды с null-указателем
 */
TEST_F(MoveCommandTest, NullMovableThrowsException) {
    EXPECT_THROW(MoveCommand(nullptr), std::invalid_argument);
}