#pragma once

#include "ICommand.hpp"
#include "CommandException.hpp"
#include <memory>
#include <vector>

/**
 * @file MacroCommand.hpp
 * @brief ������������ ��� ����������������� ���������� ������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������������ ��� ����������������� ���������� ������
  *
  * ��������� ������� Composite ��� ������.
  * ��������� ������������������ ������, �������� ���������� ��� ������ ������.
  * ������� �������� Liskov Substitution - ����� �������������� �����,
  * ��� ������������ ICommand.
  */
class MacroCommand : public ICommand {
private:
    std::vector<std::shared_ptr<ICommand>> m_commands; ///< ������������������ ������

public:
    /**
     * @brief ����������� ������������
     * @param commands ������ ������ ��� ����������������� ����������
     * @throws std::invalid_argument ���� ������ ���� ��� �������� nullptr
     */
    MacroCommand(const std::vector<std::shared_ptr<ICommand>>& commands);

    /**
     * @brief ��������� ��� ������� ���������������
     *
     * ��������� ������� � ������� �� ���������� � �������.
     * ��� ������������� ���������� ��������� ���������� � ������������ ����������.
     *
     * @throws CommandException ���� ���� �� ������ ��������� CommandException
     * @throws std::runtime_error ���� ���� �� ������ �� ����� ���� ���������
     */
    void execute() override;
};