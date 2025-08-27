#include <gtest/gtest.h>
#include "IoC.hpp"
#include "Commands.hpp"
#include "TestClasses.hpp"
#include <memory>

/**
 * @file BasicIoCTests.cpp
 * @brief Основные тесты функциональности IoC контейнера
 */

class BasicIoCTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Очистка состояния перед каждым тестом
        // В реальной реализации может понадобиться метод reset()
    }
    
    void TearDown() override {
        // Очистка после теста
    }
};

/**
 * @brief Тест регистрации и разрешения простой зависимости
 */
TEST_F(BasicIoCTests, RegisterAndResolveSimpleDependency) {
    // Создаем фабрику для TestClassA
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(std::make_shared<TestClassA>());
        }
    );
    
    auto key = std::make_shared<std::string>("TestClassA");
    std::vector<std::shared_ptr<void>> args = {key, factory};
    
    // Регистрируем зависимость через IoC.resolve
    auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
    ASSERT_NE(registerCommand, nullptr);
    registerCommand->execute();
    
    // Разрешаем зависимость
    auto resolvedObject = IoC::resolve<TestClassA>("TestClassA");
    ASSERT_NE(resolvedObject, nullptr);
    EXPECT_EQ(resolvedObject->getName(), "TestClassA");
}

/**
 * @brief Тест регистрации зависимости с инъекцией
 */
TEST_F(BasicIoCTests, RegisterAndResolveDependencyInjection) {
    // Регистрируем TestClassA
    auto factoryA = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(std::make_shared<TestClassA>());
        }
    );
    
    auto keyA = std::make_shared<std::string>("TestClassA");
    std::vector<std::shared_ptr<void>> argsA = {keyA, factoryA};
    
    auto registerCommandA = IoC::resolve<ICommand>("IoC.Register", argsA);
    registerCommandA->execute();
    
    // Регистрируем TestClassB с зависимостью от A
    auto factoryB = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            auto dependencyA = IoC::resolve<TestClassA>("TestClassA");
            return std::static_pointer_cast<void>(std::make_shared<TestClassB>(dependencyA));
        }
    );
    
    auto keyB = std::make_shared<std::string>("TestClassB");
    std::vector<std::shared_ptr<void>> argsB = {keyB, factoryB};
    
    auto registerCommandB = IoC::resolve<ICommand>("IoC.Register", argsB);
    registerCommandB->execute();
    
    // Разрешаем TestClassB
    auto resolvedB = IoC::resolve<TestClassB>("TestClassB");
    ASSERT_NE(resolvedB, nullptr);
    EXPECT_EQ(resolvedB->getName(), "TestClassB");
    
    auto dependencyA = resolvedB->getDependencyA();
    ASSERT_NE(dependencyA, nullptr);
    EXPECT_EQ(dependencyA->getName(), "TestClassA");
}

/**
 * @brief Тест обработки несуществующих зависимостей
 */
TEST_F(BasicIoCTests, ResolveNonExistentDependency) {
    EXPECT_THROW(IoC::resolve<TestClassA>("NonExistentClass"), std::runtime_error);
}

/**
 * @brief Тест интерфейсного программирования
 */
TEST_F(BasicIoCTests, InterfaceProgramming) {
    // Регистрируем реализацию интерфейса
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(
                std::static_pointer_cast<ITestInterface>(
                    std::make_shared<TestImplementation>()
                )
            );
        }
    );
    
    auto key = std::make_shared<std::string>("ITestInterface");
    std::vector<std::shared_ptr<void>> args = {key, factory};
    
    auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
    registerCommand->execute();
    
    // Разрешаем через интерфейс
    auto resolved = IoC::resolve<ITestInterface>("ITestInterface");
    ASSERT_NE(resolved, nullptr);
    EXPECT_EQ(resolved->getInterfaceInfo(), "TestImplementation");
}