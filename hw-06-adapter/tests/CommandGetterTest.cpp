#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "interfaces/CommandGetter.hpp"
#include "interfaces/IGameObject.hpp"
#include "interfaces/Vector2D.hpp"

using ::testing::_;

// Мок-реализация IGameObject для проверки вызова getProperty
class MockGameObject : public IGameObject {
public:
    MOCK_METHOD(void, setProperty, (const std::string& key, const std::any& value), (override));
    MOCK_METHOD(std::any, getProperty, (const std::string& key), (const, override));
};

TEST(CommandGetterTest, ExecutesGetProperty) {
    auto mockGameObject = std::make_shared<MockGameObject>();

    Vector2D expectedPosition{42, 84};

    // Ожидаем, что getProperty будет вызван ровно один раз с ключом "Position" и вернет expectedPosition
    EXPECT_CALL(*mockGameObject, getProperty("Position"))
        .Times(1)
        .WillOnce(testing::Return(std::any(expectedPosition)));

    // Создаем команду с объектом и ключом свойства
    CommandGetter command(mockGameObject, "Position");

    // Выполняем команду - должен вызвать getProperty у объекта
    EXPECT_NO_THROW(command.execute());

    // Получаем результат и проверяем, что он совпадает с ожидаемым
    std::any resultAny = command.getResult();
    try {
        auto result = std::any_cast<Vector2D>(resultAny);
        EXPECT_EQ(result.x, expectedPosition.x);
        EXPECT_EQ(result.y, expectedPosition.y);
    } catch (const std::bad_any_cast&) {
        FAIL() << "Result type mismatch: expected Vector2D";
    }
}
