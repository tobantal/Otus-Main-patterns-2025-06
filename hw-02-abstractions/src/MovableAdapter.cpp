#include "MovableAdapter.hpp"
#include <cmath>
#include <corecrt_math_defines.h>

MovableAdapter::MovableAdapter(std::shared_ptr<IGameObject> gameObject)
    : m_gameObject(gameObject) {
    if (!gameObject) {
        throw std::invalid_argument("Game object cannot be null");
    }
}

Vector2D MovableAdapter::getPosition() const {
    try {
        std::any positionAny = m_gameObject->getProperty("Position");
        return std::any_cast<Vector2D>(positionAny);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read position: " + std::string(e.what()));
    }
}

void MovableAdapter::setPosition(const Vector2D& position) {
    try {
        m_gameObject->setProperty("Position", position);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot set position: " + std::string(e.what()));
    }
}

Vector2D MovableAdapter::getVelocity() const {
    try {
        std::any velocityAny = m_gameObject->getProperty("Velocity");
        std::any directionAny = m_gameObject->getProperty("Direction");
        std::any directionsNumberAny = m_gameObject->getProperty("DirectionsNumber");

        int velocity = std::any_cast<int>(velocityAny);
        int direction = std::any_cast<int>(directionAny);
        int directionsNumber = std::any_cast<int>(directionsNumberAny);

        // Валидация входных данных
        if (directionsNumber <= 0) {
            throw std::runtime_error("DirectionsNumber must be positive");
        }

        if (direction < 0 || direction >= directionsNumber) {
            throw std::runtime_error("Direction must be between 0 and " +
                std::to_string(directionsNumber - 1));
        }

        // Динамическое вычисление направления
        // Преобразуем дискретное направление в радианы
        double angleRadians = (2.0 * M_PI * direction) / directionsNumber;

        // Вычисляем компоненты скорости
        double vx_double = velocity * std::cos(angleRadians);
        double vy_double = velocity * std::sin(angleRadians);

        // Округляем до ближайшего целого числа для избежания ошибок округления
        int vx = static_cast<int>(std::round(vx_double));
        int vy = static_cast<int>(std::round(vy_double));

        return Vector2D(vx, vy);

    }
    catch (const std::exception& e) {
        throw std::runtime_error("Cannot read velocity: " + std::string(e.what()));
    }
}
