#pragma once

#include <string>
#include <memory>

/**
 * @file TestClasses.hpp
 * @brief Вспомогательные классы для тестирования IoC контейнера
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Простой тестовый класс A
 */
class TestClassA {
public:
    TestClassA() = default;
    virtual ~TestClassA() = default;
    
    /**
     * @brief Возвращает имя класса
     */
    virtual std::string getName() const;
};

/**
 * @brief Тестовый класс B с зависимостью от A
 */
class TestClassB {
private:
    std::shared_ptr<TestClassA> dependencyA_;
    
public:
    /**
     * @brief Конструктор с инъекцией зависимости
     * @param a Зависимость типа TestClassA
     */
    explicit TestClassB(std::shared_ptr<TestClassA> a);
    
    /**
     * @brief Получить зависимость A
     */
    std::shared_ptr<TestClassA> getDependencyA() const;
    
    /**
     * @brief Возвращает имя класса
     */
    std::string getName() const;
};

/**
 * @brief Интерфейс для тестирования полиморфизма
 */
class ITestInterface {
public:
    virtual ~ITestInterface() = default;
    
    /**
     * @brief Виртуальный метод интерфейса
     */
    virtual std::string getInterfaceInfo() const = 0;
};

/**
 * @brief Реализация тестового интерфейса
 */
class TestImplementation : public ITestInterface {
public:
    TestImplementation() = default;
    
    /**
     * @brief Реализация метода интерфейса
     */
    std::string getInterfaceInfo() const override;
};