// tests/CommandGetterTest.cpp

#include <gtest/gtest.h>
#include <memory>
#include <any>
#include <string>
#include <unordered_map>

#include "IGameObject.hpp"
#include "CommandGetter.hpp"
#include "Vector2D.hpp"

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

TEST(CommandGetterTest, ExecutesGetProperty) {
    auto obj = std::make_shared<TestGameObject>();
    Vector2D expected{42, 84};

    // \u041f\u043e\u0434\u0433\u043e\u0442\u043e\u0432\u043a\u0430
    obj->setProperty("Position", expected);

    // \u0412\u044b\u043f\u043e\u043b\u043d\u0435\u043d\u0438\u0435
    CommandGetter command(obj, "Position");
    command.execute();

    auto resultAny = command.getResult();
    auto result = std::any_cast<Vector2D>(resultAny);
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
}
