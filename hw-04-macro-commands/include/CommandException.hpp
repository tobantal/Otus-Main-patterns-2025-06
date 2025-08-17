#pragma once

#include <stdexcept>
#include <string>

/**
 * @file CommandException.hpp
 * @brief ���������� ��� ������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ����������, ������������� ��� ������� ���������� ������
  */
class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string& message)
        : std::runtime_error(message) {}
};