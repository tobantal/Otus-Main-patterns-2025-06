#include "FuelableAdapter.hpp"

FuelableAdapter::FuelableAdapter(std::shared_ptr<IGameObject> gameObject)
    : m_gameObject(gameObject) {
    if (!gameObject) {
        throw std::invalid_argument("Game object cannot be null");
    }
}

int FuelableAdapter::getFuelLevel() const {
    try {
        std::any fuelLevelAny = m_gameObject->getProperty("FuelLevel");
        return std::any_cast<int>(fuelLevelAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read fuel level: " + std::string(e.what()));
    }
}

void FuelableAdapter::setFuelLevel(int level) {
    try {
        m_gameObject->setProperty("FuelLevel", level);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot set fuel level: " + std::string(e.what()));
    }
}

int FuelableAdapter::getFuelConsumption() const {
    try {
        std::any consumptionAny = m_gameObject->getProperty("FuelConsumption");
        return std::any_cast<int>(consumptionAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read fuel consumption: " + std::string(e.what()));
    }
}