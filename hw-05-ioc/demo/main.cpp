#include <iostream>
#include "IoC.hpp"
#include "Commands.hpp"
#include "TestClasses.hpp"
#include <thread>
#include <vector>
#include <memory>

/**
 * @file main.cpp
 * @brief Демонстрационное приложение для IoC контейнера
 */

void demonstrateBasicUsage() {
    std::cout << "=== Демонстрация базового использования IoC ===" << std::endl;
    
    // Регистрация простой зависимости
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            std::cout << "Создание экземпляра TestClassA" << std::endl;
            return std::static_pointer_cast<void>(std::make_shared<TestClassA>());
        }
    );
    
    auto key = std::make_shared<std::string>("TestClassA");
    std::vector<std::shared_ptr<void>> args = {key, factory};
    
    // Используем единственный метод IoC::resolve для регистрации
    auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
    registerCommand->execute();
    std::cout << "Зависимость TestClassA зарегистрирована" << std::endl;
    
    // Разрешение зависимости
    auto instance = IoC::resolve<TestClassA>("TestClassA");
    std::cout << "Разрешена зависимость: " << instance->getName() << std::endl;
    
    std::cout << std::endl;
}

void demonstrateDependencyInjection() {
    std::cout << "=== Демонстрация инъекции зависимостей ===" << std::endl;
    
    // Регистрация TestClassB с зависимостью от TestClassA
    auto factoryB = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            std::cout << "Создание TestClassB с инъекцией TestClassA" << std::endl;
            auto dependencyA = IoC::resolve<TestClassA>("TestClassA");
            return std::static_pointer_cast<void>(std::make_shared<TestClassB>(dependencyA));
        }
    );
    
    auto keyB = std::make_shared<std::string>("TestClassB");
    std::vector<std::shared_ptr<void>> argsB = {keyB, factoryB};
    
    auto registerCommandB = IoC::resolve<ICommand>("IoC.Register", argsB);
    registerCommandB->execute();
    std::cout << "Зависимость TestClassB зарегистрирована" << std::endl;
    
    // Разрешение с автоматической инъекцией
    auto instanceB = IoC::resolve<TestClassB>("TestClassB");
    std::cout << "Разрешена зависимость: " << instanceB->getName() << std::endl;
    std::cout << "Инъецированная зависимость: " << instanceB->getDependencyA()->getName() << std::endl;
    
    std::cout << std::endl;
}

void demonstrateScopes() {
    std::cout << "=== Демонстрация работы со скоупами ===" << std::endl;
    
    // Создание нового скоупа
    auto scopeId = std::make_shared<std::string>("DemoScope");
    std::vector<std::shared_ptr<void>> scopeArgs = {scopeId};
    
    auto createScopeCommand = IoC::resolve<ICommand>("Scopes.New", scopeArgs);
    createScopeCommand->execute();
    std::cout << "Создан скоуп: DemoScope" << std::endl;
    
    // Установка текущего скоупа
    auto setCurrentCommand = IoC::resolve<ICommand>("Scopes.Current", scopeArgs);
    setCurrentCommand->execute();
    std::cout << "Установлен текущий скоуп: DemoScope" << std::endl;
    
    // Разрешение зависимости в скоупе
    auto instance = IoC::resolve<TestClassA>("TestClassA");
    std::cout << "Зависимость разрешена в скоупе: " << instance->getName() << std::endl;
    
    std::cout << std::endl;
}

void demonstrateMultithreading() {
    std::cout << "=== Демонстрация многопоточности ===" << std::endl;
    
    const int threadCount = 3;
    std::vector<std::thread> threads;
    
    // Создаем скоупы для каждого потока
    for (int i = 0; i < threadCount; ++i) {
        auto scopeId = std::make_shared<std::string>("ThreadScope" + std::to_string(i));
        std::vector<std::shared_ptr<void>> args = {scopeId};
        
        auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
        createCommand->execute();
        std::cout << "Создан скоуп для потока " << i << ": ThreadScope" << i << std::endl;
    }
    
    // Запускаем потоки
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i]() {
            // Каждый поток устанавливает свой скоуп
            auto scopeId = std::make_shared<std::string>("ThreadScope" + std::to_string(i));
            std::vector<std::shared_ptr<void>> args = {scopeId};
            
            auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
            setCommand->execute();
            
            // Выполняем несколько операций разрешения
            for (int j = 0; j < 3; ++j) {
                auto instance = IoC::resolve<TestClassA>("TestClassA");
                std::cout << "Поток " << i << ", операция " << j << ": " 
                          << instance->getName() << std::endl;
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
    
    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Все потоки завершены" << std::endl;
    std::cout << std::endl;
}

void demonstrateInterface() {
    std::cout << "=== Демонстрация программирования через интерфейсы ===" << std::endl;
    
    // Регистрация реализации интерфейса
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            std::cout << "Создание реализации интерфейса" << std::endl;
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
    std::cout << "Интерфейс ITestInterface зарегистрирован" << std::endl;
    
    // Разрешение через интерфейс
    auto instance = IoC::resolve<ITestInterface>("ITestInterface");
    std::cout << "Разрешен интерфейс: " << instance->getInterfaceInfo() << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "IoC Container Demo Application" << std::endl;
    std::cout << "=============================" << std::endl << std::endl;
    
    try {
        demonstrateBasicUsage();
        demonstrateDependencyInjection();
        demonstrateScopes();
        demonstrateInterface();
        demonstrateMultithreading();
        
        std::cout << "=== Демонстрация завершена успешно ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}