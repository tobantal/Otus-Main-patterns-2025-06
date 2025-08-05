#pragma once

#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <unordered_map>

/**
 * @file PhotonTorpedo.hpp
 * @brief ���������� �������� �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ���������� �������� �������
  *
  * ���������� ���������� �������� ������� - �������� �������.
  * ������������ ������ ������������� �������� ��� ��������.
  */
class PhotonTorpedo : public IGameObject {
private:
    std::unordered_map<std::string, std::any> m_properties; ///< ����� ������� �������

public:
    /**
     * @brief ����������� �������� �������
     * @param position ��������� �������
     * @param velocity �������� � ���������� �����������
     */
    PhotonTorpedo(const Vector2D& position = Vector2D(0, 0),
        const Vector2D& velocity = Vector2D(0, 0));

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
     * @throws std::runtime_error ���� ������� ������ � ����������� ��������
     */
    void setProperty(const std::string& key, const std::any& value) override;
};