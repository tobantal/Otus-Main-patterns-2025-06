#include <gtest/gtest.h>
#include "MoveCommand.hpp"
#include "SpaceShip.hpp"
#include "MovableAdapter.hpp"
#include <memory>

/**
 * @brief �������� ����� ��� MoveCommand � ������������ �������
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
 * @brief ���� ��������� �������� �������
 *
 * ���������, ��� ������ � ����� (12, 5) �� ��������� (-7, 3)
 * ����� �������� ����������� � ����� (5, 8)
 *
 * ���������� ���������� ������� � 8 �������������:
 * 0: (1,0)   - ������
 * 1: (1,1)   - ������-������ (� �������������)
 * 2: (0,1)   - �����
 * 3: (-1,1)  - ������-����� (� �������������)
 * 4: (-1,0)  - �����
 * 5: (-1,-1) - ���-����� (� �������������)
 * 6: (0,-1)  - ��
 * 7: (1,-1)  - ���-������ (� �������������)
 */
TEST_F(MoveCommandTest, SuccessfulMovement) {
    // Arrange - ����������� ��������� �������
    spaceShip->setProperty("DirectionsNumber", 8);
    spaceShip->setProperty("Position", Vector2D(12, 5));
    spaceShip->setProperty("Velocity", 10);        // ������ �������� = 10
    spaceShip->setProperty("Direction", 3);        // ������-����� (-1, 1) � �������������

    // ��������� ���������� ��������
    // ����������� 3 = ������-����� = (-1, 1) * velocity * 0.7 (������������)
    // vx = 10 * (-1) * 0.7 = -7
    // vy = 10 * 1 * 0.7 = 7
    Vector2D velocity = movableAdapter->getVelocity();
    EXPECT_EQ(velocity.x, -7);
    EXPECT_EQ(velocity.y, 7);

    MoveCommand moveCommand(movableAdapter);

    // Act - ��������� ��������
    moveCommand.execute();

    // Assert - ��������� ���������
    // ����� ������� = (12, 5) + (-7, 7) = (5, 12)
    Vector2D newPosition = movableAdapter->getPosition();
    EXPECT_EQ(newPosition.x, 5);
    EXPECT_EQ(newPosition.y, 12);
}

/**
 * @brief ���� �������� �� �������� ������������
 */
TEST_F(MoveCommandTest, MovementInCardinalDirections) {
    // ���� �������� �� ������ (����������� 0)
    spaceShip->setProperty("DirectionsNumber", 8);
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Velocity", 5);
    spaceShip->setProperty("Direction", 0);  // ������

    MoveCommand moveCommand(movableAdapter);
    moveCommand.execute();

    Vector2D position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 5);
    EXPECT_EQ(position.y, 0);

    // ���� �������� �� ����� (����������� 2)
    spaceShip->setProperty("Position", Vector2D(0, 0));
    spaceShip->setProperty("Direction", 2);  // �����

    moveCommand.execute();

    position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 0);
    EXPECT_EQ(position.y, 5);

    // ���� �������� �� ����� (����������� 4)
    spaceShip->setProperty("Position", Vector2D(10, 10));
    spaceShip->setProperty("Direction", 4);  // �����

    moveCommand.execute();

    position = movableAdapter->getPosition();
    EXPECT_EQ(position.x, 5);
    EXPECT_EQ(position.y, 10);
}

/**
 * @brief ���� � �������������� ����������� ��� ��������� �������� (-7, 3)
 *
 * ������� ����������� �������� ������, ������� �������� ���������� ������ ��������
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
 * @brief ������ ���� ����������: (12, 5) + (-7, 3) = (5, 8)
 */
TEST_F(MoveCommandTest, ExactRequirementTest) {
    // Arrange - ������� ������ �������� � ������� �����������
    auto testObject = std::make_shared<TestMovableObject>(
        Vector2D(12, 5),   // ��������� �������
        Vector2D(-7, 3)    // ��������
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
 * @brief ���-����� ��� ������������ ������ ������ �������
 */
class MockMovableNoPosition : public IMovable {
public:
    Vector2D getPosition() const override {
        throw std::runtime_error("Cannot read position");
    }

    void setPosition(const Vector2D& position) override {
        // �� ������������ � ���� �����
    }

    Vector2D getVelocity() const override {
        return Vector2D(1, 1);
    }
};

/**
 * @brief ���� ������ ��� ������������� ��������� �������
 */
TEST_F(MoveCommandTest, CannotReadPosition) {
    auto mockMovable = std::make_shared<MockMovableNoPosition>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief ���-����� ��� ������������ ������ ������ ��������
 */
class MockMovableNoVelocity : public IMovable {
public:
    Vector2D getPosition() const override {
        return Vector2D(0, 0);
    }

    void setPosition(const Vector2D& position) override {
        // �� ������������ � ���� �����
    }

    Vector2D getVelocity() const override {
        throw std::runtime_error("Cannot read velocity");
    }
};

/**
 * @brief ���� ������ ��� ������������� ��������� ��������
 */
TEST_F(MoveCommandTest, CannotReadVelocity) {
    auto mockMovable = std::make_shared<MockMovableNoVelocity>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief ���-����� ��� ������������ ������ ������ �������
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
 * @brief ���� ������ ��� ������������� ���������� �������
 */
TEST_F(MoveCommandTest, CannotSetPosition) {
    auto mockMovable = std::make_shared<MockMovableCannotSetPosition>();
    MoveCommand moveCommand(mockMovable);

    EXPECT_THROW(moveCommand.execute(), std::runtime_error);
}

/**
 * @brief ���� �������� ������� � null-����������
 */
TEST_F(MoveCommandTest, NullMovableThrowsException) {
    EXPECT_THROW(MoveCommand(nullptr), std::invalid_argument);
}