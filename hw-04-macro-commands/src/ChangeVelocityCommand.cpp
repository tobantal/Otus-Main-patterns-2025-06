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
        // Получаем модуль скорости
        int velocity = m_velocityChangeable->getVelocity();

        // Если объект не движется, ничего не делаем
        if (velocity == 0) {
            return;
        }

        // Получаем текущее направление и количество направлений
        int direction = m_velocityChangeable->getDirection();
        int directionsNumber = m_velocityChangeable->getDirectionsNumber();

        // Валидация
        if (directionsNumber <= 0) {
            throw std::runtime_error("DirectionsNumber must be positive");
        }

        if (direction < 0 || direction >= directionsNumber) {
            throw std::runtime_error("Direction must be between 0 and " +
                std::to_string(directionsNumber - 1));
        }

        // Вектор скорости автоматически пересчитывается в MovableAdapter
        // на основе текущих значений Velocity, Direction и DirectionsNumber.
        // Поэтому здесь нам не нужно явно изменять вектор скорости -
        // он будет правильно вычислен при следующем вызове getVelocity() у MovableAdapter

        // Эта команда нужна для явного указания момента изменения вектора скорости
        // и может использоваться для дополнительных действий (логирование, события и т.д.)
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to change velocity: " + std::string(e.what()));
    }
}