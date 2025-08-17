#include "ChangeVelocityCommand.hpp"
#include <string>

ChangeVelocityCommand::ChangeVelocityCommand(std::shared_ptr<IVelocityChangeable> velocityChangeable)
    : m_velocityChangeable(velocityChangeable) {
    if (!velocityChangeable) {
        throw std::invalid_argument("VelocityChangeable object cannot be null");
    }
}

void ChangeVelocityCommand::execute() {
    try {
        // ѕолучаем модуль скорости
        int velocity = m_velocityChangeable->getVelocity();

        // ≈сли объект не движетс€, ничего не делаем
        if (velocity == 0) {
            return;
        }

        // ѕолучаем текущее направление и количество направлений
        int direction = m_velocityChangeable->getDirection();
        int directionsNumber = m_velocityChangeable->getDirectionsNumber();

        // ¬алидаци€
        if (directionsNumber <= 0) {
            throw std::runtime_error("DirectionsNumber must be positive");
        }

        if (direction < 0 || direction >= directionsNumber) {
            throw std::runtime_error("Direction must be between 0 and " +
                std::to_string(directionsNumber - 1));
        }

        // ¬ектор скорости автоматически пересчитываетс€ в MovableAdapter
        // на основе текущих значений Velocity, Direction и DirectionsNumber.
        // ѕоэтому здесь нам не нужно €вно измен€ть вектор скорости -
        // он будет правильно вычислен при следующем вызове getVelocity() у MovableAdapter

        // Ёта команда нужна дл€ €вного указани€ момента изменени€ вектора скорости
        // и может использоватьс€ дл€ дополнительных действий (логирование, событи€ и т.д.)
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to change velocity: " + std::string(e.what()));
    }
}