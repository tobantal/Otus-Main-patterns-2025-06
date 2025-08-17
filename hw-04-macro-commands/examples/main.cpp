#include <iostream>
#include <memory>
#include <vector>

#include "SpaceShip.hpp"
#include "MoveWithFuelCommand.hpp"
#include "RotateWithVelocityChangeCommand.hpp"
#include "MacroCommand.hpp"
#include "CommandException.hpp"

/**
 * @brief Пример использования макрокоманд в игре "Космическая битва"
 *
 * Демонстрирует:
 * - Движение с расходом топлива
 * - Поворот с изменением вектора скорости
 * - Композицию команд через макрокоманды
 * - Обработку исключений при недостатке топлива
 */
int main() {
    std::cout << "=== Space Battle: Macro Commands Demo ===" << std::endl << std::endl;

    // Создаем космический корабль
    auto spaceShip = std::make_shared<SpaceShip>();

    // Инициализируем свойства корабля
    spaceShip->setProperty("Position", Vector2D(100, 50));
    spaceShip->setProperty("Velocity", 20);           // Скорость движения
    spaceShip->setProperty("DirectionsNumber", 360);  // Система из 360 направлений
    spaceShip->setProperty("Direction", 0);           // Направление: восток (0°)
    spaceShip->setProperty("AngularVelocity", 45);    // Поворот на 45° за такт
    spaceShip->setProperty("FuelLevel", 100);         // Уровень топлива
    spaceShip->setProperty("FuelConsumption", 10);    // Расход топлива за операцию

    std::cout << "Initial state:" << std::endl;
    std::cout << "Position: ("
        << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).x << ", "
        << std::any_cast<Vector2D>(spaceShip->getProperty("Position")).y << ")"
        << std::endl;
    std::cout << "Direction: " << std::any_cast<int>(spaceShip->getProperty("Direction")) << std::endl;
    std::cout << "Fuel: " << std::any_cast<int>(spaceShip->getProperty("FuelLevel")) << std::endl;
    std::cout << std::endl;

    // Пример 1: Движение с расходом топлива
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

    // Пример 2: Поворот с изменением вектора скорости (с топливом)
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

    // Пример 3: Сложная последовательность действий
    std::cout << "=== Example 3: Complex maneuver sequence ===" << std::endl;
    try {
        // Создаем последовательность маневров
        std::vector<std::shared_ptr<ICommand>> maneuvers;

        // Поворот на 45°
        maneuvers.push_back(RotateWithVelocityChangeCommand::create(spaceShip, true));

        // Движение вперед
        maneuvers.push_back(MoveWithFuelCommand::create(spaceShip));

        // Еще один поворот
        maneuvers.push_back(RotateWithVelocityChangeCommand::create(spaceShip, true));

        // И еще движение
        maneuvers.push_back(MoveWithFuelCommand::create(spaceShip));

        // Создаем макрокоманду для выполнения всей последовательности
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

    // Пример 4: Попытка движения при недостатке топлива
    std::cout << "=== Example 4: Insufficient fuel scenario ===" << std::endl;

    // Устанавливаем низкий уровень топлива
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

    // Пример 5: Поворот неподвижного объекта
    std::cout << "=== Example 5: Rotating stationary object ===" << std::endl;

    // Создаем неподвижный объект
    auto stationaryObject = std::make_shared<SpaceShip>();
    stationaryObject->setProperty("DirectionsNumber", 360);
    stationaryObject->setProperty("Direction", 0);
    stationaryObject->setProperty("AngularVelocity", 90);
    stationaryObject->setProperty("Velocity", 0);  // Не движется

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