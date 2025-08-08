#include "RotatableAdapter.hpp"

RotatableAdapter::RotatableAdapter(std::shared_ptr<IGameObject> gameObject)
    : m_gameObject(gameObject) {
    if (!gameObject) {
        throw std::invalid_argument("Game object cannot be null");
    }
}

int RotatableAdapter::getDirection() const {
    try {
        std::any directionAny = m_gameObject->getProperty("Direction");
        return std::any_cast<int>(directionAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read direction: " + std::string(e.what()));
    }
}

void RotatableAdapter::setDirection(int direction) {
    try {
        m_gameObject->setProperty("Direction", direction);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot set direction: " + std::string(e.what()));
    }
}

int RotatableAdapter::getAngularVelocity() const {
    try {
        std::any angularVelocityAny = m_gameObject->getProperty("AngularVelocity");
        return std::any_cast<int>(angularVelocityAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read angular velocity: " + std::string(e.what()));
    }
}

int RotatableAdapter::getDirectionsNumber() const {
    try {
        std::any directionsNumberAny = m_gameObject->getProperty("DirectionsNumber");
        return std::any_cast<int>(directionsNumberAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read directions number: " + std::string(e.what()));
    }
}