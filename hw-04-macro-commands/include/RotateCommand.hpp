#pragma once

#include "ICommand.hpp"
#include "IRotatable.hpp"
#include <memory>

/**
 * @file RotateCommand.hpp
 * @brief ������� ��� ���������� �������� �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� ���������� �������� �������
  *
  * ��������� ������� Command ��� �������� ��������.
  * ������������� ������ �������� ������� �� ���� ���.
  */
class RotateCommand : public ICommand {
private:
    std::shared_ptr<IRotatable> m_rotatable; ///< ��������� �� ���������������� ������

public:
    /**
     * @brief ����������� ������� ��������
     * @param rotatable ��������� �� ������, ������� ����� ���������
     * @throws std::invalid_argument ���� rotatable ����� nullptr
     */
    explicit RotateCommand(std::shared_ptr<IRotatable> rotatable);

    /**
     * @brief ��������� ������� �������
     *
     * ������������ ������ �� ����, ������ ��� ������� �������� �� ���� ��� �������.
     * ����� ����������� = (������� ����������� + ������� ��������) % ���������� �����������.
     *
     * @throws std::runtime_error ���� �� ������� ��������� �������
     */
    void execute() override;
};