#pragma once

#include "IMovable.hpp"
#include "IGameObject.hpp"
#include <memory>
#include <cmath>

/**
 * @file MovableAdapter.hpp
 * @brief ������� ��� �������������� IGameObject � IMovable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� �������������� IGameObject � IMovable
  *
  * ��������� ������� Adapter, �������� ������������ �������������
  * ������� ������� ����� ��������� IMovable.
  * ����������� �������� ������� ��������� (�������, ��������, �����������)
  * � ��������� ��� �������� ��������.
  */
class MovableAdapter : public IMovable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< ������������ ������� ������

public:
    /**
     * @brief ����������� ��������
     * @param gameObject ������� ������ ��� ���������
     * @throws std::invalid_argument ���� gameObject ����� nullptr
     */
    explicit MovableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief �������� ������� ������� �������
     * @return ������ ������� �� �������� "Position"
     * @throws std::runtime_error ���� ������� ���������� ���������
     */
    Vector2D getPosition() const override;

    /**
     * @brief ���������� ����� ������� �������
     * @param position ����� �������
     * @throws std::runtime_error ���� ������� ���������� ����������
     */
    void setPosition(const Vector2D& position) override;

    /**
     * @brief �������� ������� �������� ������� � ���������� �����������
     *
     * ��������� �������� �� ������ �������� ���������:
     * - velocity (������ ��������)
     * - direction (�����������)
     * - directionsNumber (���������� ���������� �����������)
     *
     * @return ������ �������� � ���������� �����������
     * @throws std::runtime_error ���� �������� ���������� ���������
     */
    Vector2D getVelocity() const override;
};