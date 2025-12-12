#include <gtest/gtest.h>
#include "JsonOrderParser.hpp"
#include "UObject.hpp"
#include <string>

/**
 * @file JsonOrderParserTest.cpp
 * @brief Тесты парсера JSON приказов
 * @author Anton Tobolkin
 */

class JsonOrderParserTest : public ::testing::Test {
protected:
    JsonOrderParser parser_;
};

/**
 * @brief Тест успешного парсинга минимального приказа
 */
TEST_F(JsonOrderParserTest, ParseMinimalOrder)
{
    std::string json = R"({"id": "ship_001", "action": "StartMove"})";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.objectId, "ship_001");
    EXPECT_EQ(order.action, "StartMove");
    EXPECT_TRUE(order.isValid());
    EXPECT_NE(order.parameters, nullptr);
}

/**
 * @brief Тест парсинга приказа с целочисленным параметром
 */
TEST_F(JsonOrderParserTest, ParseOrderWithIntParameter)
{
    std::string json = R"({"id": "548", "action": "StartMove", "initialVelocity": 5})";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.objectId, "548");
    EXPECT_EQ(order.action, "StartMove");
    
    int velocity = std::any_cast<int>(order.parameters->getProperty("initialVelocity"));
    EXPECT_EQ(velocity, 5);
}

/**
 * @brief Тест парсинга приказа со строковым параметром
 */
TEST_F(JsonOrderParserTest, ParseOrderWithStringParameter)
{
    std::string json = R"({"id": "ship_001", "action": "Fire", "targetId": "enemy_001"})";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.action, "Fire");
    
    std::string targetId = std::any_cast<std::string>(order.parameters->getProperty("targetId"));
    EXPECT_EQ(targetId, "enemy_001");
}

/**
 * @brief Тест парсинга приказа с несколькими параметрами
 */
TEST_F(JsonOrderParserTest, ParseOrderWithMultipleParameters)
{
    std::string json = R"({
        "id": "ship_001",
        "action": "Configure",
        "speed": 100,
        "name": "Enterprise",
        "active": true,
        "ratio": 3.14
    })";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.objectId, "ship_001");
    EXPECT_EQ(order.action, "Configure");
    
    int speed = std::any_cast<int>(order.parameters->getProperty("speed"));
    EXPECT_EQ(speed, 100);
    
    std::string name = std::any_cast<std::string>(order.parameters->getProperty("name"));
    EXPECT_EQ(name, "Enterprise");
    
    bool active = std::any_cast<bool>(order.parameters->getProperty("active"));
    EXPECT_TRUE(active);
    
    double ratio = std::any_cast<double>(order.parameters->getProperty("ratio"));
    EXPECT_DOUBLE_EQ(ratio, 3.14);
}

/**
 * @brief Тест ошибки при отсутствии поля id
 */
TEST_F(JsonOrderParserTest, ParseOrderMissingId_ThrowsException)
{
    std::string json = R"({"action": "StartMove"})";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

/**
 * @brief Тест ошибки при отсутствии поля action
 */
TEST_F(JsonOrderParserTest, ParseOrderMissingAction_ThrowsException)
{
    std::string json = R"({"id": "ship_001"})";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

/**
 * @brief Тест ошибки при невалидном JSON
 */
TEST_F(JsonOrderParserTest, ParseInvalidJson_ThrowsException)
{
    std::string json = "not a valid json";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

/**
 * @brief Тест ошибки при пустой строке
 */
TEST_F(JsonOrderParserTest, ParseEmptyString_ThrowsException)
{
    std::string json = "";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

/**
 * @brief Тест парсинга приказа без дополнительных параметров
 */
TEST_F(JsonOrderParserTest, ParseOrderWithoutExtraParameters)
{
    std::string json = R"({"id": "ship_001", "action": "StopMove"})";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.objectId, "ship_001");
    EXPECT_EQ(order.action, "StopMove");
    EXPECT_NE(order.parameters, nullptr);
    
    // Проверяем, что параметров нет
    auto uobj = std::dynamic_pointer_cast<UObject>(order.parameters);
    EXPECT_THROW(uobj->getProperty("anyParam"), std::runtime_error);
}