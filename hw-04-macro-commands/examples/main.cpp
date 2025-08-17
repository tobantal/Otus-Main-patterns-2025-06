#include <iostream>
#include <memory>
#include <vector>

#include "SpaceShip.hpp"
#include "MoveWithFuelCommand.hpp"
#include "RotateWithVelocityChangeCommand.hpp"
#include "MacroCommand.hpp"
#include "CommandException.hpp"

/**
 * @brief ������ ������������� ����������� � ���� "����������� �����"
 *
 * �������������:
 * - �������� � �������� �������
 * - ������� � ���������� ������� ��������
 * - ���������� ������ ����� ������������
 * - ��������� ���������� ��� ���������� �������
 */
int main() {
    std::cout << "=== Space Battle: Macro Commands Demo ===" << std::endl << std::endl;

    // ������� ����������� �������
    auto spaceShip = std::make_shared<SpaceShip>();

    // �������������� �������� �������
    spaceShip->setProperty("Position", Vector2D(100, 50));
    spaceShip->setProperty("Velocity", 20);           // �������� ��������
    spaceShip->setProperty("DirectionsNumber", 360);  // ������� �� 360 �����������
    spaceShip->setProperty("Direction", 0);           // �����������: ������ (0�)
    spaceShip->setProperty("AngularVelocity", 45);    // ������� �� 45� �� ����
    spaceShip->setProperty("FuelLevel", 100);         // ������� �������
    spaceShip->setProperty("FuelConsumption", 10);    // ������ ������� �� ��������

    std::cout << "Initial state:" << std::endl;
    std::cout << "Position: ("
        << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).x << ", "
        << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).y << ")"
        << std::endl;
    std::cout << "Direction: " << std::any_cast<int>(spaceShip->getProperty("Direction")) << std::endl;
    std::cout << "Fuel: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    std::cout << std::endl;

    // ������ 1: �������� � �������� �������
    std::cout << "=== Example 1: Move with fuel consumption ===" << std::endl;
    try {
        auto moveCommand = MoveWithFuelCommand::create(spaceShip);
        moveCommand->execute();

        std::cout << "After move:" << std::endl;
        std::cout << "Position: ("
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).x << ", "
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).y << ")"
            << std::endl;
        std::cout << "Fuel: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    }
    catch (const CommandException& e) {
        std::cerr << "Command failed: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // ������ 2: ������� � ���������� ������� �������� (� ��������)
    std::cout << "=== Example 2: Rotate with velocity change and fuel ===" << std::endl;
    try {
        auto rotateCommand = RotateWithVelocityChangeCommand::create(spaceShip, true);
        rotateCommand->execute();

        std::cout << "After rotate:" << std::endl;
        std::cout << "Direction: " << std::any_cast<int>(spaceShip->getProperty("Direction")) << std::endl;
        std::cout << "Fuel: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    }
    catch (const CommandException& e) {
        std::cerr << "Command failed: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // ������ 3: ������� ������������������ ��������
    std::cout << "=== Example 3: Complex maneuver sequence ===" << std::endl;
    try {
        // ������� ������������������ ��������
        std::vector<std::shared_ptr<ICommand>> maneuvers;

        // ������� �� 45�
        maneuvers.push_back(RotateWithVelocityChangeCommand::create(spaceShip, true));

        // �������� ������
        maneuvers.push_back(MoveWithFuelCommand::create(spaceShip));

        // ��� ���� �������
        maneuvers.push_back(RotateWithVelocityChangeCommand::create(spaceShip, true));

        // � ��� ��������
        maneuvers.push_back(MoveWithFuelCommand::create(spaceShip));

        // ������� ������������ ��� ���������� ���� ������������������
        MacroCommand complexManeuver(maneuvers);
        complexManeuver.execute();

        std::cout << "After complex maneuver:" << std::endl;
        std::cout << "Position: ("
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).x << ", "
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).y << ")"
            << std::endl;
        std::cout << "Direction: " << std::any_cast<int>(spaceShip->getProperty("Direction")) << std::endl;
        std::cout << "Fuel: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    }
    catch (const CommandException& e) {
        std::cerr << "Command failed: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // ������ 4: ������� �������� ��� ���������� �������
    std::cout << "=== Example 4: Insufficient fuel scenario ===" << std::endl;

    // ������������� ������ ������� �������
    spaceShip->setProperty("FuelLevel", 5);
    std::cout << "Setting fuel to 5 (consumption is 10)" << std::endl;

    try {
        auto moveCommand = MoveWithFuelCommand::create(spaceShip);
        moveCommand->execute();
        std::cout << "Move succeeded (shouldn't happen)" << std::endl;
    }
    catch (const CommandException& e) {
        std::cout << "Expected exception caught: " << e.what() << std::endl;
        std::cout << "Position unchanged: ("
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).x << ", "
            << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).y << ")"
            << std::endl;
        std::cout << "Fuel unchanged: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // ������ 5: ������� ������������ �������
    std::cout << "=== Example 5: Rotating stationary object ===" << std::endl;

    // ������� ����������� ������
    auto stationaryObject = std::make_shared<SpaceShip>();
    stationaryObject->setProperty("DirectionsNumber", 360);
    stationaryObject->setProperty("Direction", 0);
    stationaryObject->setProperty("AngularVelocity", 90);
    stationaryObject->setProperty("Velocity", 0);  // �� ��������

    try {
        auto rotateCommand = RotateWithVelocityChangeCommand::create(stationaryObject, false);
        rotateCommand->execute();

        std::cout << "Stationary object rotated successfully" << std::endl;
        std::cout << "New direction: " << std::any_cast<int>(stationaryObject->getProperty("Direction")) << std::endl;
        std::cout << "Velocity remains: " << std::any_cast<int>(stationaryObject->getProperty("Velocity")) << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << std::endl << "=== Demo completed ===" << std::endl;

    return 0;
}