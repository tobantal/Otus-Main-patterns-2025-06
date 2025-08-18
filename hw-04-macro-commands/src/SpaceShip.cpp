#include "SpaceShip.hpp"

SpaceShip::SpaceShip() = default;

std::any SpaceShip::getProperty(const std::string& key) const {
    auto it = m_properties.find(key);
    if (it == m_properties.end()) {
        throw std::runtime_error("Property '" + key + "' not found");
    }
    return it->second;
}

void SpaceShip::setProperty(const std::string& key, const std::any& value) {
    // Валидация значений
    if (key == "Direction") {
        int direction = std::any_cast<int>(value);
		auto dn = getProperty("DirectionsNumber"); // DirectionsNumber должно быть установлено заранее
        int directionsNumber = std::any_cast<int>(dn);
        if (direction < 0 || direction >= directionsNumber) {
            throw std::runtime_error("Direction must be between 0 and " + std::to_string(directionsNumber - 1));
        }
    }

    m_properties[key] = value;
}