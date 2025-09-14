/**
 * @file TestSimpleInterfaceParser.cpp
 * @brief Unit-тесты для SimpleInterfaceParser
 * @author Anton Tobolkin
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/parsers/SimpleInterfaceParser.hpp"
#include "../include/models/InterfaceInfo.hpp"
#include "../include/models/MethodInfo.hpp"
#include <fstream>

using ::testing::_;
using ::testing::Return;
using ::testing::Eq;

/**
 * @brief Тестовый класс для SimpleInterfaceParser
 */
class SimpleInterfaceParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser = std::make_unique<SimpleInterfaceParser>();
    }

    void TearDown() override {
        parser.reset();
    }

    std::unique_ptr<SimpleInterfaceParser> parser;
};

/**
 * @brief Тест парсинга простого интерфейса
 */
TEST_F(SimpleInterfaceParserTest, ParseSimpleInterface) {
    std::string testInterface = R"(
        class IMovable {
        public:
            virtual Vector getPosition() const = 0;
            virtual void setPosition(const Vector& pos) = 0;
            virtual Vector getVelocity() const = 0;
        };
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "IMovable");
    EXPECT_EQ(interfaces[0].methods.size(), 3);
    
    // Проверяем первый метод (getter)
    const auto& getPosition = interfaces[0].methods[0];
    EXPECT_EQ(getPosition.name, "getPosition");
    EXPECT_EQ(getPosition.returnType, "Vector");
    EXPECT_TRUE(getPosition.isConst);
    EXPECT_TRUE(getPosition.isPureVirtual);
    EXPECT_TRUE(getPosition.parameters.empty());
    EXPECT_TRUE(getPosition.isGetter());
    
    // Проверяем второй метод (setter)
    const auto& setPosition = interfaces[0].methods[1];
    EXPECT_EQ(setPosition.name, "setPosition");
    EXPECT_EQ(setPosition.returnType, "void");
    EXPECT_FALSE(setPosition.isConst);
    EXPECT_TRUE(setPosition.isPureVirtual);
    EXPECT_EQ(setPosition.parameters.size(), 1);
    EXPECT_TRUE(setPosition.isSetter());
    
    // Проверяем третий метод (getter)
    const auto& getVelocity = interfaces[0].methods[2];
    EXPECT_EQ(getVelocity.name, "getVelocity");
    EXPECT_EQ(getVelocity.returnType, "Vector");
    EXPECT_TRUE(getVelocity.isConst);
    EXPECT_TRUE(getVelocity.isPureVirtual);
    EXPECT_TRUE(getVelocity.parameters.empty());
    EXPECT_TRUE(getVelocity.isGetter());
}

/**
 * @brief Тест парсинга интерфейса с namespace
 */
TEST_F(SimpleInterfaceParserTest, ParseInterfaceWithNamespace) {
    std::string testInterface = R"(
        namespace SpaceShip::Operations {
            class IRotatable {
            public:
                virtual int getDirection() const = 0;
                virtual void setDirection(int direction) = 0;
                virtual int getAngularVelocity() const = 0;
            };
        }
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "IRotatable");
    EXPECT_EQ(interfaces[0].namespaceName, "SpaceShip::Operations");
    EXPECT_EQ(interfaces[0].getFullName(), "SpaceShip::Operations::IRotatable");
    EXPECT_EQ(interfaces[0].methods.size(), 3);
}

/**
 * @brief Тест парсинга интерфейса с void методами
 */
TEST_F(SimpleInterfaceParserTest, ParseInterfaceWithVoidMethods) {
    std::string testInterface = R"(
        class IFinishable {
        public:
            virtual void finish() = 0;
            virtual void reset() = 0;
            virtual void cleanup() = 0;
        };
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "IFinishable");
    EXPECT_EQ(interfaces[0].methods.size(), 3);
    
    for (const auto& method : interfaces[0].methods) {
        EXPECT_EQ(method.returnType, "void");
        EXPECT_TRUE(method.parameters.empty());
        EXPECT_TRUE(method.isVoidMethod());
        EXPECT_FALSE(method.isGetter());
        EXPECT_FALSE(method.isSetter());
    }
}

/**
 * @brief Тест парсинга интерфейса с кастомными реализациями
 */
TEST_F(SimpleInterfaceParserTest, ParseInterfaceWithCustomImplementations) {
    std::string testInterface = R"(
        class IMovable {
        public:
            virtual Vector getPosition() const = 0;
            
            /**
             * @custom_impl MovableImplementations::complexVelocityCalculation
             */
            virtual Vector getVelocity() const = 0;
            
            virtual void setPosition(const Vector& pos) = 0;
        };
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "IMovable");
    EXPECT_EQ(interfaces[0].methods.size(), 3);
    
    // Проверяем метод без кастомной реализации
    const auto& getPosition = interfaces[0].methods[0];
    EXPECT_FALSE(getPosition.hasCustomImplementation());
    
    // Проверяем метод с кастомной реализацией
    const auto& getVelocity = interfaces[0].methods[1];
    EXPECT_TRUE(getVelocity.hasCustomImplementation());
    EXPECT_EQ(getVelocity.customImpl.className, "MovableImplementations");
    EXPECT_EQ(getVelocity.customImpl.methodName, "complexVelocityCalculation");
    EXPECT_EQ(getVelocity.customImpl.getFullReference(), "MovableImplementations::complexVelocityCalculation");
    
    // Проверяем третий метод без кастомной реализации
    const auto& setPosition = interfaces[0].methods[2];

    EXPECT_FALSE(setPosition.hasCustomImplementation());
}

/**
 * @brief Тест парсинга нескольких интерфейсов
 */
TEST_F(SimpleInterfaceParserTest, ParseMultipleInterfaces) {
    std::string testContent = R"(
        class IMovable {
        public:
            virtual Vector getPosition() const = 0;
            virtual void setPosition(const Vector& pos) = 0;
        };
        
        class IRotatable {
        public:
            virtual int getDirection() const = 0;
            virtual void setDirection(int direction) = 0;
        };
        
        // Это не чистый интерфейс - есть не виртуальные методы
        // генерим адаптер, считается, что пользователь осознает, что ему нужны также не виртальные методы
        class NotPureInterface {
        public:
            virtual void virtualMethod() = 0;
            void regularMethod() {}
        };
    )";

    auto interfaces = parser->parseContent(testContent);

    // Должны найти только 2 интерфейса
    ASSERT_EQ(interfaces.size(), 3);
    
    // Проверяем первый интерфейс
    EXPECT_EQ(interfaces[0].className, "IMovable");
    EXPECT_EQ(interfaces[0].methods.size(), 2);
    
    // Проверяем второй интерфейс
    EXPECT_EQ(interfaces[1].className, "IRotatable");
    EXPECT_EQ(interfaces[1].methods.size(), 2);

    // Проверяем третий интерфейс
    EXPECT_EQ(interfaces[2].className, "NotPureInterface");
    EXPECT_EQ(interfaces[2].methods.size(), 1);
}

/**
 * @brief Тест парсинга методов с различными параметрами
 */
TEST_F(SimpleInterfaceParserTest, ParseMethodsWithDifferentParameters) {
    std::string testInterface = R"(
        class IComplexInterface {
        public:
            virtual void noParams() = 0;
            virtual void oneParam(int value) = 0;
            virtual void twoParams(int a, const std::string& b) = 0;
            virtual int complexParam(const std::vector<std::shared_ptr<Object>>& objects) = 0;
        };
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    const auto& methods = interfaces[0].methods;
    ASSERT_EQ(methods.size(), 4);
    
    // Метод без параметров
    EXPECT_EQ(methods[0].name, "noParams");
    EXPECT_TRUE(methods[0].parameters.empty());
    
    // Метод с одним параметром
    EXPECT_EQ(methods[1].name, "oneParam");
    EXPECT_EQ(methods[1].parameters.size(), 1);
    EXPECT_EQ(methods[1].parameters[0], "int value");
    
    // Метод с двумя параметрами
    EXPECT_EQ(methods[2].name, "twoParams");
    EXPECT_EQ(methods[2].parameters.size(), 2);
    EXPECT_EQ(methods[2].parameters[0], "int a");
    EXPECT_EQ(methods[2].parameters[1], "const std::string& b");
    
    // Метод со сложным параметром
    EXPECT_EQ(methods[3].name, "complexParam");
    EXPECT_EQ(methods[3].parameters.size(), 1);
    EXPECT_EQ(methods[3].parameters[0], "const std::vector<std::shared_ptr<Object>>& objects");
}

/**
 * @brief Тест обработки пустого содержимого
 */
TEST_F(SimpleInterfaceParserTest, ParseEmptyContent) {
    std::string emptyContent = "";
    auto interfaces = parser->parseContent(emptyContent);
    EXPECT_TRUE(interfaces.empty());
}

/**
 * @brief Тест обработки содержимого без интерфейсов
 */
TEST_F(SimpleInterfaceParserTest, ParseContentWithoutInterfaces) {
    std::string noInterfaceContent = R"(
        class RegularClass {
        public:
            void method1() {}
            int method2() { return 0; }
        };
        
        struct RegularStruct {
            int value;
            void setValue(int v) { value = v; }
        };
    )";

    auto interfaces = parser->parseContent(noInterfaceContent);
    EXPECT_TRUE(interfaces.empty());
}

/**
 * @brief Тест обработки некорректного синтаксиса
 */
TEST_F(SimpleInterfaceParserTest, ParseInvalidSyntax) {
    std::string invalidContent = R"(
        class InvalidClass {
        public:
            virtual void method1() = 0
            // Пропущена точка с запятой
            virtual int method2() const = 0;
        };
    )";

    // Парсер должен быть устойчив к некорректному синтаксису
    EXPECT_NO_THROW({
        auto interfaces = parser->parseContent(invalidContent);
    });
}

/**
 * @brief Тест парсинга интерфейса с комплексными типами возврата
 */
TEST_F(SimpleInterfaceParserTest, ParseInterfaceWithComplexReturnTypes) {
    std::string testInterface = R"(
        class IComplexReturns {
        public:
            virtual std::shared_ptr<Object> getObject() const = 0;
            virtual const std::vector<int>& getVector() const = 0;
            virtual std::unique_ptr<std::map<std::string, int>> getMap() const = 0;
        };
    )";

    auto interfaces = parser->parseContent(testInterface);

    ASSERT_EQ(interfaces.size(), 1);
    const auto& methods = interfaces[0].methods;
    ASSERT_EQ(methods.size(), 3);
    
    EXPECT_EQ(methods[0].returnType, "std::shared_ptr<Object>");
    EXPECT_EQ(methods[1].returnType, "const std::vector<int>&");
    EXPECT_EQ(methods[2].returnType, "std::unique_ptr<std::map<std::string, int>>");
}

/**
 * @brief Интеграционный тест парсинга файла
 */
TEST_F(SimpleInterfaceParserTest, ParseFileIntegration) {
    // Создаем временный файл для тестирования
    std::string tempFileName = "test_interface.hpp";
    std::string testContent = R"(
        #pragma once
        
        namespace TestNamespace {
            class ITestInterface {
            public:
                virtual ~ITestInterface() = default;
                virtual int getValue() const = 0;
                virtual void setValue(int value) = 0;
            };
        }
    )";
    
    // Записываем содержимое в файл
    std::ofstream tempFile(tempFileName);
    tempFile << testContent;
    tempFile.close();
    
    // Парсим файл
    auto interfaces = parser->parseFile(tempFileName);
    
    // Проверяем результат
    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "ITestInterface");
    EXPECT_EQ(interfaces[0].namespaceName, "TestNamespace");
    EXPECT_EQ(interfaces[0].methods.size(), 2); // Деструктор не должен парситься
    
    // Удаляем временный файл
    std::remove(tempFileName.c_str());
}

/**
 * @brief Тест парсинга файла, которого не существует
 */
TEST_F(SimpleInterfaceParserTest, ParseNonExistentFile) {
    EXPECT_THROW({
        parser->parseFile("non_existent_file.hpp");
    }, std::runtime_error);
}