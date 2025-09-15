#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "interfaces/CommandSetter.hpp"
#include "interfaces/IGameObject.hpp"
#include "interfaces/Vector2D.hpp"

using ::testing::_;

// Mock класса IGameObject для проверки вызова метода setProperty
class MockGameObject : public IGameObject {
public:
    MOCK_METHOD(void, setProperty, (const std::string& key, const std::any& value), (override));
    MOCK_METHOD(std::any, getProperty, (const std::string& key), (const, override));
};

TEST(CommandSetterTest, ExecutesSetProperty) {
    auto mockGameObject = std::make_shared<MockGameObject>();
    Vector2D position{42, 84};

    // Ожидаем, что метод setProperty будет вызван ровно один раз с ключом "Position" и правильным значением
    EXPECT_CALL(*mockGameObject, setProperty("Position", _))
        .Times(1)
        .WillOnce([&position](const std::string&, const std::any& val) {
            try {
                auto casted = std::any_cast<Vector2D>(val);
                EXPECT_EQ(casted.x, position.x);
                EXPECT_EQ(casted.y, position.y);
            } catch (const std::bad_any_cast&) {
                FAIL() << "Parameter type mismatch: expected Vector2D";
            }
        });

    // Создаем команду
    CommandSetter command(mockGameObject, "Position", position);

    // Выполняем команду, проверить, что вызывается моковский метод setProperty с указанными параметрами
    command.execute();
}
