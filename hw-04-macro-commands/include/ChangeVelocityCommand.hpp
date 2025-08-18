#pragma once

#include "ICommand.hpp"
#include "IVelocityChangeable.hpp"
#include <memory>

/**
 * @file ChangeVelocityCommand.hpp
 * @brief ������� ��� ��������� ������� �������� ��� ��������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� ��������� ������� �������� ��� ��������
  *
  * ������������ ������ ���������� �������� � ������ �������� �������.
  * ��������� ������ ��������, �� �������� ����������� � ������������
  * � ����� ������������ �������.
  * ���������, ��� �� ������ ����������������� ������ ��������.
  */
class ChangeVelocityCommand : public ICommand {
private:
    std::shared_ptr<IVelocityChangeable> m_velocityChangeable;

public:
    /**
     * @brief ����������� ������� ��������� ��������
     * @param velocityChangeable ��������� �� ������ � ���������� ���������
     * @throws std::invalid_argument ���� velocityChangeable ����� nullptr
     */
    explicit ChangeVelocityCommand(std::shared_ptr<IVelocityChangeable> velocityChangeable);

    /**
     * @brief ��������� ��������� ������� ��������
     *
     * ������������� ������ �������� �� ������ �������� ����������� �������.
     * ���� ������ �� �������� (�������� = 0), ������� ������ �� ������.
     *
     * @throws std::runtime_error ���� �� ������� ��������� ��������
     */
    void execute() override;
};