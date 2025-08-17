#pragma once

#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file FuelableAdapter.hpp
 * @brief ������� ��� �������������� IGameObject � IFuelable
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� �������������� IGameObject � IFuelable
  *
  * ��������� ������� Adapter, �������� ������������ �������������
  * ������� ������� ����� ��������� IFuelable.
  */
class FuelableAdapter : public IFuelable {
private:
    std::shared_ptr<IGameObject> m_gameObject; ///< ������������ ������� ������

public:
    /**
     * @brief ����������� ��������
     * @param gameObject ������� ������ ��� ���������
     * @throws std::invalid_argument ���� gameObject ����� nullptr
     */
    explicit FuelableAdapter(std::shared_ptr<IGameObject> gameObject);

    /**
     * @brief �������� ������� ������� �������
     * @return ������� ������� �� �������� "FuelLevel"
     * @throws std::runtime_error ���� ������� ������� ���������� ���������
     */
    int getFuelLevel() const override;

    /**
     * @brief ���������� ����� ������� �������
     * @param level ����� ������� �������
     * @throws std::runtime_error ���� ������� ������� ���������� ����������
     */
    void setFuelLevel(int level) override;

    /**
     * @brief �������� ������ ������� �� ��������
     * @return ������ ������� �� �������� "FuelConsumption"
     * @throws std::runtime_error ���� ������ ������� ���������� ���������
     */
    int getFuelConsumption() const override;
};