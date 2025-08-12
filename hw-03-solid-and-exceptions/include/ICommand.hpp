#pragma once

#include <stdexcept>

/**
 * @file ICommand.hpp
 * @brief ��������� ������� �� �������� Command
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ������� �� �������� Command
  *
  * ������������ ������������ �������� � �������, ��� ���������
  * ��������������� �������� � ���������� ���������, ������� �������
  * � ������� � ������������ ������ ��������.
  */
class ICommand {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~ICommand() = default;

    /**
     * @brief ��������� �������
     * @throws std::runtime_error ���� ������� ���������� ���������
     */
    virtual void execute() = 0;
};