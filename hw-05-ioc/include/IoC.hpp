#pragma once

#include "IocContainer.hpp"
#include "Commands.hpp"
#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>

/**
 * @file IoC.hpp
 * @brief Главный статический интерфейс IoC контейнера
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Главный статический интерфейс IoC контейнера
 * 
 * Предоставляет единый интерфейс для всех операций с зависимостями.
 * Реализует паттерн Facade для упрощения работы с контейнером.
 */
class IoC {
public:
    /**
     * @brief Единственный метод для всех операций с IoC контейнером
     * 
     * Этот метод служит точкой входа для:
     * - Регистрации зависимостей
     * - Разрешения зависимостей  
     * - Управления скоупами
     * - Выполнения команд
     * 
     * @tparam T Тип возвращаемого объекта
     * @param key Ключ операции или зависимости
     * @param args Аргументы для операции
     * @return Указатель на объект типа T
     * @throws std::runtime_error при ошибках выполнения
     */
    template<typename T>
    static std::shared_ptr<T> resolve(const std::string& key, 
                                      const std::vector<std::shared_ptr<void>>& args = {});
};


template<typename T>
std::shared_ptr<T> IoC::resolve(const std::string& key, 
                                const std::vector<std::shared_ptr<void>>& args) {

    std::cout << ">>> IoC::resolve: key='" << key << "', type=" << typeid(T).name() << std::endl;
    
    if (key == "IoC.Register") {
        std::cout << ">>> Processing IoC.Register branch" << std::endl;
        if constexpr (std::is_base_of<ICommand, T>::value) {
            if (args.size() >= 2) {
                // Используем reinterpret_pointer_cast для приведения void* к конкретным типам
                auto dependencyKey = std::reinterpret_pointer_cast<std::string>(args[0]);
                auto factory = std::reinterpret_pointer_cast<IocContainer::FactoryFunction>(args[1]);
                auto registerCommand = std::make_shared<RegisterCommand>(*dependencyKey, *factory);
                std::cout << ">>> Returning RegisterCommand" << std::endl;
                return std::static_pointer_cast<T>(registerCommand);
            }
            throw std::runtime_error("IoC.Register requires dependency key and factory function");
        } else {
            throw std::logic_error("IoC.Register key is only valid for ICommand types");
        }
    }

    if (key == "Scopes.New") {
        std::cout << ">>> Processing Scopes.New branch" << std::endl;
        if constexpr (std::is_base_of<ICommand, T>::value) {
            if (!args.empty()) {
                auto scopeId = std::reinterpret_pointer_cast<std::string>(args[0]);
                auto createScopeCommand = std::make_shared<CreateScopeCommand>(*scopeId);
                std::cout << ">>> Returning CreateScopeCommand" << std::endl;
                return std::static_pointer_cast<T>(createScopeCommand);
            }
            throw std::runtime_error("Scopes.New requires scope ID");
        } else {
            throw std::logic_error("Scopes.New key is only valid for ICommand types");
        }
    }

    if (key == "Scopes.Current") {
        std::cout << ">>> Processing Scopes.Current branch" << std::endl;
        if constexpr (std::is_base_of<ICommand, T>::value) {
            if (!args.empty()) {
                auto scopeId = std::reinterpret_pointer_cast<std::string>(args[0]);
                auto setScopeCommand = std::make_shared<SetCurrentScopeCommand>(*scopeId);
                std::cout << ">>> Returning SetCurrentScopeCommand" << std::endl;
                return std::static_pointer_cast<T>(setScopeCommand);
            }
            throw std::runtime_error("Scopes.Current requires scope ID");
        } else {
            throw std::logic_error("Scopes.Current key is only valid for ICommand types");
        }
    }

     std::cout << ">>> Calling IocContainer::getInstance().resolve<T>('" << key << "')" << std::endl;
    // Обычное разрешение зависимостей
    return IocContainer::getInstance().resolve<T>(key);
}

