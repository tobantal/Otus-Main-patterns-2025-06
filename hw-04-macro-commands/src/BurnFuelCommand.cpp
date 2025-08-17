#include "BurnFuelCommand.hpp"

BurnFuelCommand::BurnFuelCommand(std::shared_ptr<IFuelable> fuelable)
    : m_fuelable(fuelable) {
    if (!fuelable) {
        throw std::invalid_argument("Fuelable object cannot be null");
    }
}

void BurnFuelCommand::execute() {
    try {
        int currentFuel = m_fuelable->getFuelLevel();
        int consumption = m_fuelable->getFuelConsumption();
        int newFuelLevel = currentFuel - consumption;

        // Устанавливаем новый уровень топлива
        m_fuelable->setFuelLevel(newFuelLevel);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to burn fuel: " + std::string(e.what()));
    }
}