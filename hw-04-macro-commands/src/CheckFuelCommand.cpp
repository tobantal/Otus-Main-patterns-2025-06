#include "CheckFuelCommand.hpp"

CheckFuelCommand::CheckFuelCommand(std::shared_ptr<IFuelable> fuelable)
    : m_fuelable(fuelable) {
    if (!fuelable) {
        throw std::invalid_argument("Fuelable object cannot be null");
    }
}

void CheckFuelCommand::execute() {
    try {
        int currentFuel = m_fuelable->getFuelLevel();
        int consumption = m_fuelable->getFuelConsumption();

        if (currentFuel < consumption) {
            throw CommandException("Insufficient fuel: current level " +
                std::to_string(currentFuel) + " is less than consumption " +
                std::to_string(consumption));
        }
    }
    catch (const CommandException& e) {
        throw e; // Пробрасываем CommandException дальше
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to check fuel: " + std::string(e.what()));
    }
}