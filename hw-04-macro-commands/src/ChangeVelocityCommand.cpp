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
        // �������� ������ ��������
        int velocity = m_velocityChangeable->getVelocity();

        // ���� ������ �� ��������, ������ �� ������
        if (velocity == 0) {
            return;
        }

        // �������� ������� ����������� � ���������� �����������
        int direction = m_velocityChangeable->getDirection();
        int directionsNumber = m_velocityChangeable->getDirectionsNumber();

        // ���������
        if (directionsNumber <= 0) {
            throw std::runtime_error("DirectionsNumber must be positive");
        }

        if (direction < 0 || direction >= directionsNumber) {
            throw std::runtime_error("Direction must be between 0 and " +
                std::to_string(directionsNumber - 1));
        }

        // ������ �������� ������������� ��������������� � MovableAdapter
        // �� ������ ������� �������� Velocity, Direction � DirectionsNumber.
        // ������� ����� ��� �� ����� ���� �������� ������ �������� -
        // �� ����� ��������� �������� ��� ��������� ������ getVelocity() � MovableAdapter

        // ��� ������� ����� ��� ������ �������� ������� ��������� ������� ��������
        // � ����� �������������� ��� �������������� �������� (�����������, ������� � �.�.)
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to change velocity: " + std::string(e.what()));
    }
}