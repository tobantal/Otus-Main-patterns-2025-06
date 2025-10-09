// tests/TestDefaultGetterSetterRegistry.cpp

#include "gtest/gtest.h"
#include "CommandGetter.hpp"
#include "CommandSetter.hpp"
#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <memory>
#include <string>
#include <any>
#include <unordered_map>

// Простая реализация IGameObject для теста
class SimpleGameObject : public IGameObject {
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

TEST(CommandTest, GetterSetterBasicTest) {
    auto obj = std::make_shared<SimpleGameObject>();
    
    // Тест CommandSetter
    Vector2D pos{10, 20};
    CommandSetter setter(obj, "Position");
    setter.setValue(pos);
    setter.execute();
    
    auto stored = std::any_cast<Vector2D>(obj->getProperty("Position"));
    EXPECT_EQ(stored.x, 10);
    EXPECT_EQ(stored.y, 20);
    
    // Тест CommandGetter
    CommandGetter getter(obj, "Position");
    getter.execute();
    
    auto result = std::any_cast<Vector2D>(getter.getResult());
    EXPECT_EQ(result.x, 10);
    EXPECT_EQ(result.y, 20);
}