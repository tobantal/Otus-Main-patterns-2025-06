#pragma once

#include "ICommand.hpp"
#include <memory>

/**
 * @file RetryCommand.hpp
 * @brief ������� ��� ���������� ���������� ������ �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief �������-��������� ��� ���������� ���������� ������ �������
  *
  * ��������� ������� Decorator ��� ���������� �����������
  * ���������� ���������� ������� ��� ��������� � ����.
  */
class RetryCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param command ������� ��� ���������� ����������
     */
    explicit RetryCommand(std::shared_ptr<ICommand> command);

    /**
     * @brief ����������
     */
    ~RetryCommand() override = default;

    /**
     * @brief ��������� �������
     * @throws ������������ ���������� �� ���������� �������
     */
    void execute() override;

    /**
     * @brief �������� ���������� �������
     * @return ��������� �� ���������� �������
     */
    std::shared_ptr<ICommand> getInnerCommand() const;

private:
    /**
     * @brief ������� ��� ���������� ����������
     */
    std::shared_ptr<ICommand> command_;
};