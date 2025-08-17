#pragma once

#include "ICommand.hpp"
#include "IFuelable.hpp"
#include <memory>

/**
 * @file BurnFuelCommand.hpp
 * @brief ������� ��� �������� �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� �������� �������
  *
  * ��������� ������� Command ��� �������� �������� �������.
  * ��������� ���������� ������� �� �������� �������.
  */
class BurnFuelCommand : public ICommand {
private:
    std::shared_ptr<IFuelable> m_fuelable; ///< ��������� �� ������ � ��������

public:
    /**
     * @brief ����������� ������� �������� �������
     * @param fuelable ��������� �� ������ � ��������
     * @throws std::invalid_argument ���� fuelable ����� nullptr
     */
    explicit BurnFuelCommand(std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief ��������� �������� �������
     *
     * ��������� ������� ������� ������� �� �������� ������� �������.
     * ����� ������� = ������� ������� - ������.
     *
     * @throws std::runtime_error ���� �� ������� ��������� ��������
     */
    void execute() override;
};