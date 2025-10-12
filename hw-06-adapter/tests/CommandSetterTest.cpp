// tests/CommandSetterTest.cpp

#include <gtest/gtest.h>
#include <memory>
#include <any>
#include <string>
#include <unordered_map>

#include "IGameObject.hpp"
#include "CommandSetter.hpp"
#include "Vector2D.hpp"

// Тестовая реализация IGameObject
class TestGameObject : public IGameObject {
public:
    void setProperty(const std::string& key, const std::any& value) override {
        props_[key] = value;
    }

    std::any getProperty(const std::string& key) const override {
        auto it = props_.find(key);
        if (it == props_.end()) {
            throw std::runtime_error("Property not found: " + key);
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, std::any> props_;
};

TEST(CommandSetterTest, ExecutesSetProperty) {
    auto obj = std::make_shared<TestGameObject>();
    Vector2D position{42, 84};

    CommandSetter command(obj, "Position");
    command.setValue(position);
    command.execute();

    auto storedAny = obj->getProperty("Position");
    auto stored = std::any_cast<Vector2D>(storedAny);
    EXPECT_EQ(stored.x, position.x);
    EXPECT_EQ(stored.y, position.y);
}
