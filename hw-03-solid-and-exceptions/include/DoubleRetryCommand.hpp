#pragma once

#include "ICommand.hpp"
#include <memory>


/**
 * @brief ������� ��� �������� ����������
 *
 * ������������ ��� ������������� ����, ��� �������
 * ��� ���� ��������� ���� ���.
 */
class DoubleRetryCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param command ������� ��� ���������� ����������
     */
    explicit DoubleRetryCommand(std::shared_ptr<ICommand> command);

    /**
     * @brief ����������
     */
    ~DoubleRetryCommand() override = default;

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