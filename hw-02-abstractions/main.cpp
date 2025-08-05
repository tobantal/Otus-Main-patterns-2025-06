#include <iostream>
#include "SpaceShip.hpp"
#include "MoveCommand.hpp"
#include "RotateCommand.hpp"
#include "MovableAdapter.hpp"
#include "RotatableAdapter.hpp"

/**
 * @brief ���������������� ���������
 *
 * ���������� �������� ����������� ������� �������� ��������
 */
int main() {
    std::cout << "Space Battle - Movement System Demo\n";
    std::cout << "===================================\n\n";

    try {
        // ������� ����������� �������
        auto spaceShip = std::make_shared<SpaceShip>();

        // ����������� ��������� ���������
        spaceShip->setProperty("Position", Vector2D(10, 5));
        spaceShip->setProperty("DirectionsNumber", 360); // 360 ��������
        spaceShip->setProperty("Velocity", 8);
        spaceShip->setProperty("Direction", 45); // 45 ��������
        spaceShip->setProperty("AngularVelocity", 30);

        std::cout << "Initial spaceship state:\n";
        auto pos = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
        auto vel = std::any_cast<int>(spaceShip->getProperty("Velocity"));
        auto dir = std::any_cast<int>(spaceShip->getProperty("Direction"));
        std::cout << "Position: (" << pos.x << ", " << pos.y << ")\n";
        std::cout << "Velocity: " << vel << "\n";
        std::cout << "Direction: " << dir << " degrees\n\n";

        // ������� ��������
        auto movableAdapter = std::make_shared<MovableAdapter>(spaceShip);
        auto rotatableAdapter = std::make_shared<RotatableAdapter>(spaceShip);

        // ������� �������
        MoveCommand moveCommand(movableAdapter);
        RotateCommand rotateCommand(rotatableAdapter);

        // ��������� ��������
        std::cout << "Executing move command...\n";
        moveCommand.execute();

        pos = std::any_cast<Vector2D>(spaceShip->getProperty("Position"));
        std::cout << "New position: (" << pos.x << ", " << pos.y << ")\n\n";

        // ��������� �������
        std::cout << "Executing rotate command...\n";
        rotateCommand.execute();

        dir = std::any_cast<int>(spaceShip->getProperty("Direction"));
        std::cout << "New direction: " << dir << " degrees\n\n";

        std::cout << "Demo completed successfully!\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}