#include "MoveCommand.hpp"

MoveCommand::MoveCommand(std::shared_ptr<IMovable> movable)
    : m_movable(movable) {
    if (!movable) {
        throw std::invalid_argument("Movable object cannot be null");
    }
}

void MoveCommand::execute() {
    try {
        Vector2D currentPosition = m_movable->getPosition();
        Vector2D velocity = m_movable->getVelocity();
        Vector2D newPosition = currentPosition + velocity;
        m_movable->setPosition(newPosition);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to execute move command: " + std::string(e.what()));
    }
}