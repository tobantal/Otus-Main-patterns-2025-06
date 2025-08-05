#pragma once

#include "IRotatable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file RotatableAdapter.hpp
 * @brief ������� ��� �������������� IGameObject � IRotatable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� �������������� IGameObject � IRotatable
  *
  * ��������� ������� Adapter, �������� ������������ �������������
  * ������� ������� ����� ��������� IRotatable.
  */
class RotatableAdapter : public IRotatable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< ������������ ������� ������

public:
    /**
     * @brief ����������� ��������
     * @param gameObject ������� ������ ��� ���������
     * @throws std::invalid_argument ���� gameObject ����� nullptr
     */
    explicit RotatableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief �������� ������� ����������� �������
     * @return ����������� �� �������� "Direction"
     * @throws std::runtime_error ���� ����������� ���������� ���������
     */
    int getDirection() const override;

    /**
     * @brief ���������� ����� ����������� �������
     * @param direction ����� �����������
     * @throws std::runtime_error ���� ����������� ���������� ����������
     */
    void setDirection(int direction) override;

    /**
     * @brief �������� ������� �������� �������
     * @return ������� �������� �� �������� "AngularVelocity"
     * @throws std::runtime_error ���� ������� �������� ���������� ���������
     */
    int getAngularVelocity() const override;

    /**
     * @brief �������� ����� ���������� ��������� �����������
     * @return ���������� ����������� �� �������� "DirectionsNumber"
     * @throws std::runtime_error ���� �������� ���������� ���������
     */
    int getDirectionsNumber() const override;
};