#pragma once

#include "ICommand.hpp"
#include "IMovable.hpp"
#include <memory>

/**
 * @file MoveCommand.hpp
 * @brief ������� ��� ���������� �������������� ��������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� ���������� �������������� ��������
  *
  * ��������� ������� Command ��� �������� ��������.
  * ������������� ������ ����������� ������� �� ���� ���.
  */
class MoveCommand : public ICommand {
private:
    std::shared_ptr<IMovable> m_movable; ///< ��������� �� ���������� ������

public:
    /**
     * @brief ����������� ������� ��������
     * @param movable ��������� �� ������, ������� ����� �����������
     * @throws std::invalid_argument ���� movable ����� nullptr
     */
    explicit MoveCommand(std::shared_ptr<IMovable> movable);

    /**
     * @brief ��������� �������� �������
     *
     * ���������� ������ �� ����������, ������ ��� �������� �� ���� ��� �������.
     * ����� ������� = ������� ������� + ��������.
     *
     * @throws std::runtime_error ���� �� ������� ��������� ��������
     */
    void execute() override;
};