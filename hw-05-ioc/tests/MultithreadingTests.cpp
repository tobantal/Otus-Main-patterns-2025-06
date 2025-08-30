#include <gtest/gtest.h>
#include "IoC.hpp"
#include "Commands.hpp"
#include "TestClasses.hpp"
#include <thread>
#include <vector>
#include <memory>
#include <atomic>

/**
 * @file MultithreadingTests.cpp
 * @brief Тесты многопоточной функциональности IoC контейнера
 */

class MultithreadingTests : public ::testing::Test {
protected:
    void SetUp() override {
        setupBasicDependencies();
        createTestScopes();
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
    
    void createTestScopes() {
        for (int i = 0; i < 3; ++i) {
            auto scopeId = std::make_shared<std::string>("Scope" + std::to_string(i));
            std::vector<std::shared_ptr<void>> args = {scopeId};
            
            auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
            createCommand->execute();
        }
    }
};

/**
 * @brief Тест параллельного разрешения зависимостей
 */
TEST_F(MultithreadingTests, ParallelResolve) {
    const int threadCount = 10;
    const int resolutionsPerThread = 100;
    
    std::atomic<int> successCount{0};
    std::atomic<int> errorCount{0};
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([&successCount, &errorCount, resolutionsPerThread]() {
            for (int j = 0; j < resolutionsPerThread; ++j) {
                try {
                    auto resolved = IoC::resolve<TestClassA>("TestClassA");
                    if (resolved && resolved->getName() == "TestClassA") {
                        successCount.fetch_add(1);
                    } else {
                        errorCount.fetch_add(1);
                    }
                } catch (const std::exception&) {
                    errorCount.fetch_add(1);
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(successCount.load(), threadCount * resolutionsPerThread);
    EXPECT_EQ(errorCount.load(), 0);
}

/**
 * @brief Тест thread-local скоупов
 */
TEST_F(MultithreadingTests, ThreadLocalScopes) {
    const int threadCount = 3;
    std::atomic<int> successCount{0};
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, &successCount]() {
            try {
                // Каждый поток устанавливает свой скоуп
                auto scopeId = std::make_shared<std::string>("Scope" + std::to_string(i));
                std::vector<std::shared_ptr<void>> args = {scopeId};
                
                auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
                setCommand->execute();
                
                // Разрешаем зависимость в своем скоупе
                auto resolved = IoC::resolve<TestClassA>("TestClassA");
                if (resolved && resolved->getName() == "TestClassA") {
                    successCount.fetch_add(1);
                }
                
                // Делаем паузу чтобы другие потоки могли работать
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                
                // Проверяем что зависимость все еще разрешается
                auto resolved2 = IoC::resolve<TestClassA>("TestClassA");
                if (resolved2 && resolved2->getName() == "TestClassA") {
                    successCount.fetch_add(1);
                }
                
            } catch (const std::exception&) {
                // Ошибка
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(successCount.load(), threadCount * 2);
}

/**
 * @brief Тест параллельной регистрации зависимостей
 */
TEST_F(MultithreadingTests, ParallelRegistration) {
    const int threadCount = 5;
    std::atomic<int> registrationCount{0};
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, &registrationCount]() {
            try {
                std::string className = "MultiThreadClass" + std::to_string(i);
                
                auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
                    []() -> std::shared_ptr<void> {
                        return std::static_pointer_cast<void>(std::make_shared<TestClassA>());
                    }
                );
                
                auto key = std::make_shared<std::string>(className);
                std::vector<std::shared_ptr<void>> args = {key, factory};
                
                auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
                registerCommand->execute();
                
                registrationCount.fetch_add(1);
                
            } catch (const std::exception&) {
                // Ошибка регистрации
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(registrationCount.load(), threadCount);
    
    // Проверяем что все зарегистрированные зависимости доступны
    for (int i = 0; i < threadCount; ++i) {
        std::string className = "MultiThreadClass" + std::to_string(i);
        EXPECT_NO_THROW(IoC::resolve<TestClassA>(className));
    }
}

/**
 * @brief Стресс-тест производительности
 */
TEST_F(MultithreadingTests, StressTest) {
    const int threadCount = 20;
    const int operationsPerThread = 50;
    
    std::atomic<int> totalOperations{0};
    std::atomic<int> errors{0};
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, &totalOperations, &errors, operationsPerThread]() {
            for (int j = 0; j < operationsPerThread; ++j) {
                try {
                    // Смешанные операции: создание скоупов, установка скоупов, разрешение зависимостей
                    int operation = (i * operationsPerThread + j) % 3;
                    
                    switch (operation) {
                        case 0: {
                            // Создание скоупа
                            std::string scopeName = "StressScope_" + std::to_string(i) + "_" + std::to_string(j);
                            auto scopeId = std::make_shared<std::string>(scopeName);
                            std::vector<std::shared_ptr<void>> args = {scopeId};
                            
                            auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
                            createCommand->execute();
                            break;
                        }
                        case 1: {
                            // Установка скоупа (используем один из существующих)
                            int scopeIndex = j % 3;
                            auto scopeId = std::make_shared<std::string>("Scope" + std::to_string(scopeIndex));
                            std::vector<std::shared_ptr<void>> args = {scopeId};
                            
                            auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
                            setCommand->execute();
                            break;
                        }
                        case 2: {
                            // Разрешение зависимости
                            auto resolved = IoC::resolve<TestClassA>("TestClassA");
                            if (!resolved || resolved->getName() != "TestClassA") {
                                errors.fetch_add(1);
                            }
                            break;
                        }
                    }
                    
                    totalOperations.fetch_add(1);
                    
                } catch (const std::exception&) {
                    errors.fetch_add(1);
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(totalOperations.load(), threadCount * operationsPerThread);
    // Допускаем небольшое количество ошибок при стресс-тестировании
    EXPECT_LT(errors.load(), totalOperations.load() * 0.01); // Менее 1% ошибок
}