#pragma once

#include <memory>
#include <exception>

// Forward declaration of ICommand to avoid circular dependency
class ICommand;

/**
 * @file IExceptionHandler.hpp
 * @brief ��������� ����������� ����������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ��� ��������� ����������, ����������� ��� ���������� ������
  *
  * ��������� ����������� ��������� ��������� ��������� ����������
  * � ������������ � ��������� Open-Closed (SOLID)
  */
class IExceptionHandler {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~IExceptionHandler() = default;

    /**
     * @brief ���������� ����������
     * @param command �������, ������� ��������� ����������
     * @param exception ������������� ����������
     */
    virtual void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) = 0;
};