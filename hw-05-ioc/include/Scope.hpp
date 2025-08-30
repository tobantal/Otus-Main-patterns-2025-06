#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <mutex>
#include <iostream>

/**
 * @file Scope.hpp
 * @brief Класс для управления скоупами зависимостей
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Класс для управления скоупами зависимостей
 * 
 * Предоставляет изоляцию зависимостей по скоупам,
 * поддерживает иерархию скоупов и thread-safe операции.
 */
class Scope {
public:
    using FactoryFunction = std::function<std::shared_ptr<void>()>;
    
    /**
     * @brief Конструктор скоупа
     * @param parent Родительский скоуп (может быть nullptr)
     */
    explicit Scope(std::shared_ptr<Scope> parent = nullptr);
    
    /**
     * @brief Деструктор
     */
    ~Scope() = default;
    
    /**
     * @brief Регистрирует фабрику для создания зависимости
     * @param key Ключ зависимости
     * @param factory Фабричная функция
     */
    void registerDependency(const std::string& key, FactoryFunction factory);
    
    /**
     * @brief Разрешает зависимость по ключу
     * @tparam T Тип зависимости
     * @param key Ключ зависимости
     * @return Указатель на созданный объект
     * @throws std::runtime_error если зависимость не найдена
     */
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& key);
    
    /**
     * @brief Получает родительский скоуп
     * @return Указатель на родительский скоуп
     */
    std::shared_ptr<Scope> getParent() const;

private:
    std::unordered_map<std::string, FactoryFunction> dependencies_;
    std::shared_ptr<Scope> parent_;
    mutable std::mutex mutex_;
};

template<typename T>
std::shared_ptr<T> Scope::resolve(const std::string& key) {
    std::cout << ">>> Scope::resolve: key='" << key << "', type=" << typeid(T).name() << std::endl;
    
    std::cout << ">>> Scope::resolve: acquiring mutex for '" << key << "'" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << ">>> Scope::resolve: mutex acquired for '" << key << "'" << std::endl;
    
    auto it = dependencies_.find(key);
    if (it != dependencies_.end()) {
        std::cout << ">>> Scope::resolve: found in current scope, calling factory for '" << key << "'" << std::endl;
        auto result = it->second();
        std::cout << ">>> Scope::resolve: factory completed for '" << key << "'" << std::endl;
        return std::static_pointer_cast<T>(result);
    }
    
    if (parent_) {
        std::cout << ">>> Scope::resolve: not found in current scope, checking parent for '" << key << "'" << std::endl;
        return parent_->resolve<T>(key);
    }
    
    std::cout << ">>> Scope::resolve: '" << key << "' not found anywhere in scope chain" << std::endl;
    throw std::runtime_error("Dependency not found: " + key);
}
