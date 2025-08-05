#include "PhotonTorpedo.hpp"
#include "IMovable.hpp"
#include <memory>

PhotonTorpedo::PhotonTorpedo(const Vector2D& position, const Vector2D& velocity) {
    // Торпеда использует декартову систему координат напрямую
    m_properties["Position"] = position;
    m_properties["Velocity"] = velocity;  // Хранится как Vector2D, а не как полярные координаты
}

std::any PhotonTorpedo::getProperty(const std::string& key) const {
    auto it = m_properties.find(key);
    if (it == m_properties.end()) {
        throw std::runtime_error("Property '" + key + "' not found");
    }
    return it->second;
}

void PhotonTorpedo::setProperty(const std::string& key, const std::any& value) {
    // Торпеда поддерживает только Position и Velocity
    if (key != "Position" && key != "Velocity") {
        throw std::runtime_error("Property '" + key + "' is not supported for PhotonTorpedo");
    }

    m_properties[key] = value;
}

/**
 * @brief Специальный адаптер для торпеды с декартовой системой координат
 */
class TorpedoMovableAdapter : public IMovable {
private:
    std::shared_ptr<PhotonTorpedo> m_torpedo;

public:
    explicit TorpedoMovableAdapter(std::shared_ptr<PhotonTorpedo> torpedo)
        : m_torpedo(torpedo) {
        if (!torpedo) {
            throw std::invalid_argument("Torpedo cannot be null");
        }
    }

    Vector2D getPosition() const override {
        try {
            std::any positionAny = m_torpedo->getProperty("Position");
            return std::any_cast<Vector2D>(positionAny);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Cannot read position: " + std::string(e.what()));
        }
    }

    void setPosition(const Vector2D& position) override {
        try {
            m_torpedo->setProperty("Position", position);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Cannot set position: " + std::string(e.what()));
        }
    }

    Vector2D getVelocity() const override {
        try {
            std::any velocityAny = m_torpedo->getProperty("Velocity");
            return std::any_cast<Vector2D>(velocityAny);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Cannot read velocity: " + std::string(e.what()));
        }
    }
};