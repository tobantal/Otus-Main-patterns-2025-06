#pragma once

#include "IVelocityChangeable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file VelocityChangeableAdapter.hpp
 * @brief Адаптер для изменения скорости
 * @author Space Battle Team
 * @version 1.0
 */

class VelocityChangeableAdapter : public IVelocityChangeable {
private:
    std::shared_ptr<IGameObject> m_gameObject;

public:
    explicit VelocityChangeableAdapter(std::shared_ptr<IGameObject> gameObject);

    int getVelocity() const override;
    int getDirection() const override;
    int getDirectionsNumber() const override;
};