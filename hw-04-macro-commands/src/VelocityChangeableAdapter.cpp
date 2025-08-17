#include "VelocityChangeableAdapter.hpp"

VelocityChangeableAdapter::VelocityChangeableAdapter(std::shared_ptr<IGameObject> gameObject)
    : m_gameObject(gameObject) {
    if (!gameObject) {
        throw std::invalid_argument("Game object cannot be null");
    }
}

int VelocityChangeableAdapter::getVelocity() const {
    try {
        std::any velocityAny = m_gameObject->getProperty("Velocity");
        return std::any_cast<int>(velocityAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read velocity: " + std::string(e.what()));
    }
}

int VelocityChangeableAdapter::getDirection() const {
    try {
        std::any directionAny = m_gameObject->getProperty("Direction");
        return std::any_cast<int>(directionAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read direction: " + std::string(e.what()));
    }
}

int VelocityChangeableAdapter::getDirectionsNumber() const {
    try {
        std::any directionsNumberAny = m_gameObject->getProperty("DirectionsNumber");
        return std::any_cast<int>(directionsNumberAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read directions number: " + std::string(e.what()));
    }
}