#pragma once

#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <unordered_map>

/**
 * @file SpaceShip.hpp
 * @brief ���������� ������������ �������
 * @author Space Battle Team
 * @version 2.0
 */

 /**
  * @brief ���������� ������������ �������
  *
  * ���������� ���������� �������� ������� - ������������ �������.
  * ������������ ��� ����������� �������� ��� �������� � ��������.
  * ���������� ������������� ���������� � ���������� ������� �����������.
  */
class SpaceShip : public IGameObject {
private:
    std::unordered_map<std::string, std::any> m_properties; ///< ����� ������� �������

public:
	/// @brief ����������� �� ���������
    SpaceShip();

    /**
     * @brief �������� �������� ��������
     * @param key ���� ��������
     * @return �������� ��������
     * @throws std::runtime_error ���� �������� �� �������
     */
    std::any getProperty(const std::string& key) const override;

    /**
     * @brief ���������� �������� ��������
     * @param key ���� ��������
     * @param value ����� ��������
     * @throws std::runtime_error ���� �������� �������� �� ������ ��� �������� �����������
     */
    void setProperty(const std::string& key, const std::any& value) override;
};