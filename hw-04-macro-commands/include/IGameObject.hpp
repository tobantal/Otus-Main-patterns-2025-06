#pragma once

#include <any>
#include <string>
#include <stdexcept>

/**
 * @file IGameObject.hpp
 * @brief ������������� ��������� �������� �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������������� ��������� �������� �������
  *
  * ������������� ����� ������ ������� � ��������� ������ �������� �������
  * ����� ������� "��������". ��� ������������ ������������� ������� -
  * ����� ��������� ����� ���� �������� ��� ��������� ������������� ����.
  */
class IGameObject {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~IGameObject() = default;

    /**
     * @brief �������� �������� �������� �������
     * @param key ���� ��������
     * @return �������� ��������
     * @throws std::runtime_error ���� �������� �� ������� ��� ����������
     */
    virtual std::any getProperty(const std::string& key) const = 0;

    /**
     * @brief ���������� �������� �������� �������
     * @param key ���� ��������
     * @param value ����� ��������
     * @throws std::runtime_error ���� �������� ������ ����������
     */
    virtual void setProperty(const std::string& key, const std::any& value) = 0;
};