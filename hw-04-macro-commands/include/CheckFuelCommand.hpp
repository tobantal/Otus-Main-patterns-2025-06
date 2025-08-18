#pragma once

#include "ICommand.hpp"
#include "IFuelable.hpp"
#include "CommandException.hpp"
#include <memory>

/**
 * @file CheckFuelCommand.hpp
 * @brief ������� ��� �������� ������� �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� �������� ������� �������
  *
  * ��������� ������� Command ��� �������� �������� �������.
  * ����������� ����������, ���� ������� ������������ ��� ��������.
  */
class CheckFuelCommand : public ICommand {
private:
    std::shared_ptr<IFuelable> m_fuelable; ///< ��������� �� ������ � ��������

public:
    /**
     * @brief ����������� ������� �������� �������
     * @param fuelable ��������� �� ������ � ��������
     * @throws std::invalid_argument ���� fuelable ����� nullptr
     */
    explicit CheckFuelCommand(std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief ��������� ������� ������������ ���������� �������
     *
     * ���������, ��� ������� ������� ������� �� ������ ������� �������.
     *
     * @throws CommandException ���� ������� ������������
     * @throws std::runtime_error ���� �� ������� ��������� ��������
     */
    void execute() override;
};