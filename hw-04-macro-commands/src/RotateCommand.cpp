#include "RotateCommand.hpp"

RotateCommand::RotateCommand(std::shared_ptr<IRotatable> rotatable)
    : m_rotatable(rotatable) {
    if (!rotatable) {
        throw std::invalid_argument("Rotatable object cannot be null");
    }
}

void RotateCommand::execute() {
    try {
        int currentDirection = m_rotatable->getDirection();
        int angularVelocity = m_rotatable->getAngularVelocity();
        int directionsNumber = m_rotatable->getDirectionsNumber();

        int newDirection = (currentDirection + angularVelocity) % directionsNumber;

        // Обработка отрицательных значений для корректного модуля
        if (newDirection < 0) {
            newDirection += directionsNumber;
        }

        m_rotatable->setDirection(newDirection);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to execute rotate command: " + std::string(e.what()));
    }
}