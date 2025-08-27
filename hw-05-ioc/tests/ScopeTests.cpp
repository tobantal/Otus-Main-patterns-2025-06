#include <gtest/gtest.h>
#include "IoC.hpp"
#include "Commands.hpp"
#include "TestClasses.hpp"
#include <memory>

/**
 * @file ScopeTests.cpp
 * @brief Тесты функциональности скоупов
 */

class ScopeTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка базовых зависимостей
        setupBasicDependencies();
    }
    
private:
    void setupBasicDependencies() {
        auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void> {
                return std::static_pointer_cast<void>(std::make_shared<TestClassA>());
            }
        );
        
        auto key = std::make_shared<std::string>("TestClassA");
        std::vector<std::shared_ptr<void>> args = {key, factory};
        
        auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
        registerCommand->execute();
    }
};

/**
 * @brief Тест создания нового скоупа
 */
TEST_F(ScopeTests, CreateNewScope) {
    auto scopeId = std::make_shared<std::string>("TestScope");
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    auto createScopeCommand = IoC::resolve<ICommand>("Scopes.New", args);
    ASSERT_NE(createScopeCommand, nullptr);
    
    EXPECT_NO_THROW(createScopeCommand->execute());
}

/**
 * @brief Тест установки текущего скоупа
 */
TEST_F(ScopeTests, SetCurrentScope) {
    // Создаем скоуп
    auto scopeId = std::make_shared<std::string>("TestScope");
    std::vector<std::shared_ptr<void>> createArgs = {scopeId};
    
    auto createScopeCommand = IoC::resolve<ICommand>("Scopes.New", createArgs);
    createScopeCommand->execute();
    
    // Устанавливаем как текущий
    std::vector<std::shared_ptr<void>> setArgs = {scopeId};
    auto setCurrentCommand = IoC::resolve<ICommand>("Scopes.Current", setArgs);
    ASSERT_NE(setCurrentCommand, nullptr);
    
    EXPECT_NO_THROW(setCurrentCommand->execute());
}

/**
 * @brief Тест разрешения зависимостей в скоупе
 */
TEST_F(ScopeTests, ResolveInScope) {
    // Создаем и устанавливаем скоуп
    auto scopeId = std::make_shared<std::string>("TestScope");
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
    createCommand->execute();
    
    auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
    setCommand->execute();
    
    // Разрешаем зависимость (должна работать через глобальные зависимости)
    auto resolved = IoC::resolve<TestClassA>("TestClassA");
    ASSERT_NE(resolved, nullptr);
    EXPECT_EQ(resolved->getName(), "TestClassA");
}

/**
 * @brief Тест установки несуществующего скоупа
 */
TEST_F(ScopeTests, SetNonExistentScope) {
    auto scopeId = std::make_shared<std::string>("NonExistentScope");
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
    EXPECT_THROW(setCommand->execute(), std::runtime_error);
}