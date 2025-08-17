#include "RotateWithVelocityChangeCommand.hpp"
#include "CheckFuelCommand.hpp"
#include "RotateCommand.hpp"
#include "BurnFuelCommand.hpp"
#include "ChangeVelocityCommand.hpp"
#include "RotatableAdapter.hpp"
#include "VelocityChangeableAdapter.hpp"
#include "FuelableAdapter.hpp"

RotateWithVelocityChangeCommand::RotateWithVelocityChangeCommand(
    std::shared_ptr<IRotatable> rotatable,
    std::shared_ptr<IVelocityChangeable> velocityChangeable,
    std::shared_ptr<IFuelable> fuelable)
    : MacroCommand([&]() {
    std::vector<std::shared_ptr<ICommand>> commands;

    // Если есть топливо, добавляем проверку
    if (fuelable) {
        commands.push_back(std::make_shared<CheckFuelCommand>(fuelable));
    }

    // Всегда выполняем поворот
    commands.push_back(std::make_shared<RotateCommand>(rotatable));

    // Если есть топливо, сжигаем его
    if (fuelable) {
        commands.push_back(std::make_shared<BurnFuelCommand>(fuelable));
    }

    // Если объект может изменять скорость, изменяем вектор скорости
    if (velocityChangeable) {
        commands.push_back(std::make_shared<ChangeVelocityCommand>(velocityChangeable));
    }

    return commands;
        }()) {
    if (!rotatable) {
        throw std::invalid_argument("Rotatable object cannot be null");
    }
}

std::shared_ptr<RotateWithVelocityChangeCommand>
RotateWithVelocityChangeCommand::create(
    std::shared_ptr<IGameObject> gameObject,
    bool useFuel) {

    auto rotatable = std::make_shared<RotatableAdapter>(gameObject);

    // Проверяем, движется ли объект
    std::shared_ptr<IVelocityChangeable> velocityChangeable = nullptr;
    try {
        // Пытаемся получить скорость
        std::any velocityAny = gameObject->getProperty("Velocity");
        int velocity = std::any_cast<int>(velocityAny);

        // Если объект движется, создаем адаптер для изменения скорости
        if (velocity > 0) {
            velocityChangeable = std::make_shared<VelocityChangeableAdapter>(gameObject);
        }
    }
    catch (...) {
        // Объект не имеет скорости или она недоступна
        velocityChangeable = nullptr;
    }

    // Создаем адаптер для топлива, если нужно
    std::shared_ptr<IFuelable> fuelable = nullptr;
    if (useFuel) {
        fuelable = std::make_shared<FuelableAdapter>(gameObject);
    }

    return std::make_shared<RotateWithVelocityChangeCommand>(
        rotatable, velocityChangeable, fuelable);
}