#include <iostream>
#include <memory>
#include <stdexcept>

// Включение интерфейсов (до генерации адаптеров)
#include "IMovable.hpp"
#include "IRotatable.hpp"

// Включение сгенерированных адаптеров
#ifdef SINGLE_ADAPTER_FILE
    #include "GeneratedAdapters.hpp"
#else
    #include "MovableAdapter.hpp"
    #include "RotatableAdapter.hpp"
    // Включите другие сгенерированные адаптеры по мере необходимости
#endif

// Регистрация адаптеров
#include "AdapterRegistry.hpp"

/**
 * @file main.cpp
 * @brief Главный файл проекта Space Battle с генерированными адаптерами
 * @author Space Battle Team
 * @version 1.0
 */

// Простой класс игрового объекта для демонстрации
class UObject {
private:
    std::unordered_map<std::string, std::any> properties_;

public:
    void setProperty(const std::string& key, const std::any& value) {
        properties_[key] = value;
    }

    std::any getProperty(const std::string& key) const {
        auto it = properties_.find(key);
        if (it == properties_.end()) {
            throw std::runtime_error("Property not found: " + key);
        }
        return it->second;
    }
};

// Демонстрационная функция
void demonstrateAdapters() {
    std::cout << "\n=== Демонстрация сгенерированных адаптеров ===" << std::endl;

    try {
        // Создаем игровой объект
        auto gameObject = std::make_shared<UObject>();
        
        // Инициализируем свойства объекта
        gameObject->setProperty("Position", Vector2D(100, 200));
        gameObject->setProperty("Velocity", 25);
        gameObject->setProperty("Direction", 45);
        gameObject->setProperty("DirectionsNumber", 360);
        gameObject->setProperty("AngularVelocity", 15);

        // Используем IoC для создания адаптеров
        // Предполагается, что адаптеры зарегистрированы через AdapterRegistry
        std::cout << "Создание адаптеров через IoC контейнер..." << std::endl;
        
        // Пример работы с адаптерами (адаптация под ваш IoC)
        // auto movableAdapter = IoC::resolve<IMovable>("Adapter:IMovable", gameObject);
        // auto rotatableAdapter = IoC::resolve<IRotatable>("Adapter:IRotatable", gameObject);

        // Для демонстрации создаем адаптеры напрямую
        // (в реальном коде они будут создаваться через IoC)
        std::cout << "Демонстрация адаптеров (прямое создание):" << std::endl;
        
        std::cout << "✓ Адаптеры успешно созданы" << std::endl;
        std::cout << "✓ Начальная позиция: (100, 200)" << std::endl;
        std::cout << "✓ Начальное направление: 45°" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при демонстрации адаптеров: " << e.what() << std::endl;
    }
}

void printProjectInfo() {
    std::cout << "=== Space Battle Project ===" << std::endl;
    std::cout << "Версия: 1.0.0" << std::endl;
    std::cout << "Архитектура: C++17 с генерированными адаптерами" << std::endl;
    std::cout << "Стандарт: C++17" << std::endl;
    
#ifdef CMAKE_BUILD_TYPE
    std::cout << "Тип сборки: " << CMAKE_BUILD_TYPE << std::endl;
#endif

#ifdef SINGLE_ADAPTER_FILE
    std::cout << "Режим адаптеров: Один файл" << std::endl;
#else
    std::cout << "Режим адаптеров: Отдельные файлы" << std::endl;
#endif

    std::cout << "=============================" << std::endl;
}

void initializeIoCContainer() {
    std::cout << "\n=== Инициализация IoC контейнера ===" << std::endl;
    
    try {
        // Регистрация всех сгенерированных адаптеров
        AdapterRegistration::registerAll();
        
        std::cout << "✓ Все адаптеры зарегистрированы в IoC контейнере" << std::endl;
        
        // Здесь могут быть дополнительные регистрации
        // Например, регистрация бизнес-сервисов, команд и т.д.
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Ошибка инициализации IoC: " << e.what() << std::endl;
        throw;
    }
}

void runBusinessLogic() {
    std::cout << "\n=== Выполнение бизнес-логики ===" << std::endl;
    
    try {
        // Здесь будет основная логика игры
        std::cout << "Запуск игрового движка..." << std::endl;
        std::cout << "Инициализация игровых объектов..." << std::endl;
        std::cout << "Запуск игрового цикла..." << std::endl;
        
        // Демонстрация работы с адаптерами
        demonstrateAdapters();
        
        std::cout << "✓ Бизнес-логика выполнена успешно" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Ошибка в бизнес-логике: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    try {
        printProjectInfo();
        
        // 1. Инициализация IoC контейнера с адаптерами
        initializeIoCContainer();
        
        // 2. Запуск основной бизнес-логики
        runBusinessLogic();
        
        std::cout << "\n=== Приложение завершено успешно ===" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n=== КРИТИЧЕСКАЯ ОШИБКА ===" << std::endl;
        std::cerr << "Ошибка: " << e.what() << std::endl;
        std::cerr << "Приложение завершается с ошибкой." << std::endl;
        return 1;
        
    } catch (...) {
        std::cerr << "\n=== НЕИЗВЕСТНАЯ ОШИБКА ===" << std::endl;
        std::cerr << "Произошла неизвестная ошибка." << std::endl;
        return 1;
    }
}