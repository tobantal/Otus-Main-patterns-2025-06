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

    // ���� ���� �������, ��������� ��������
    if (fuelable) {
        commands.push_back(std::make_shared<CheckFuelCommand>(fuelable));
    }

    // ������ ��������� �������
    commands.push_back(std::make_shared<RotateCommand>(rotatable));

    // ���� ���� �������, ������� ���
    if (fuelable) {
        commands.push_back(std::make_shared<BurnFuelCommand>(fuelable));
    }

    // ���� ������ ����� �������� ��������, �������� ������ ��������
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

    // ���������, �������� �� ������
    std::shared_ptr<IVelocityChangeable> velocityChangeable = nullptr;
    try {
        // �������� �������� ��������
        std::any velocityAny = gameObject->getProperty("Velocity");
        int velocity = std::any_cast<int>(velocityAny);

        // ���� ������ ��������, ������� ������� ��� ��������� ��������
        if (velocity > 0) {
            velocityChangeable = std::make_shared<VelocityChangeableAdapter>(gameObject);
        }
    }
    catch (...) {
        // ������ �� ����� �������� ��� ��� ����������
        velocityChangeable = nullptr;
    }

    // ������� ������� ��� �������, ���� �����
    std::shared_ptr<IFuelable> fuelable = nullptr;
    if (useFuel) {
        fuelable = std::make_shared<FuelableAdapter>(gameObject);
    }

    return std::make_shared<RotateWithVelocityChangeCommand>(
        rotatable, velocityChangeable, fuelable);
}