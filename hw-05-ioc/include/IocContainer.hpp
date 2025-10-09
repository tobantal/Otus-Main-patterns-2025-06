#pragma once

#include "Scope.hpp"
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <iostream>

/**
 * @file IocContainer.hpp
 * @brief Внутренняя реализация IoC контейнера
 * @author Anton Tobolkin
 * @version 1.1
 * Управляет глобальными зависимостями и скоупами.
 * Поддерживает thread-local скоупы для многопоточного доступа.
 */
class IocContainer {
public:
    using FactoryFunction = std::function<std::shared_ptr<void>()>;
    
    /**
     * @brief Получить экземпляр контейнера (singleton)
     * @return Ссылка на единственный экземпляр контейнера
     */
    static IocContainer& getInstance();
    
    /**
     * @brief Регистрирует глобальную зависимость
     * @param key Ключ зависимости
     * @param factory Фабричная функция
     */
    void registerGlobalDependency(const std::string& key, FactoryFunction factory);
    
    /**
     * @brief Создает новый скоуп
     * @param scopeId Идентификатор скоупа
     * @return Указатель на созданный скоуп
     */
    std::shared_ptr<Scope> createScope(const std::string& scopeId);
    
    /**
     * @brief Устанавливает текущий скоуп для текущего потока
     * @param scopeId Идентификатор скоупа
     * @throws std::runtime_error если скоуп не найден
     */
    void setCurrentScope(const std::string& scopeId);
    
    /**
     * @brief Получает текущий скоуп для текущего потока
     * @return Указатель на текущий скоуп или nullptr
     */
    std::shared_ptr<Scope> getCurrentScope() const;
    
    /**
     * @brief Разрешает зависимость
     * @tparam T Тип зависимости
     * @param key Ключ зависимости
     * @return Указатель на созданный объект
     * @throws std::runtime_error если зависимость не найдена
     */
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& key);

private:
    IocContainer() = default;
    ~IocContainer() = default;
    IocContainer(const IocContainer&) = delete;
    IocContainer& operator=(const IocContainer&) = delete;
    
    std::unordered_map<std::string, FactoryFunction> globalDependencies_;
    std::unordered_map<std::string, std::shared_ptr<Scope>> scopes_;
    thread_local static std::shared_ptr<Scope> currentScope_;
    mutable std::recursive_mutex globalMutex_;
};

template<typename T>
std::shared_ptr<T> IocContainer::resolve(const std::string& key) {
    std::cout << ">>> IocContainer::resolve: key='" << key << "', type=" << typeid(T).name() << std::endl;
    
    // Сначала пробуем найти в текущем скоупе
    if (currentScope_) {
        std::cout << ">>> IocContainer::resolve: checking currentScope for '" << key << "'" << std::endl;
        try {
            auto result = currentScope_->resolve<T>(key);
            std::cout << ">>> IocContainer::resolve: found in currentScope for '" << key << "'" << std::endl;
            return result;
        } catch (const std::runtime_error&) {
            std::cout << ">>> IocContainer::resolve: not found in currentScope for '" << key << "', trying global" << std::endl;
            // Если не найдено в скоупе, ищем в глобальных
        }
    } else {
        std::cout << ">>> IocContainer::resolve: no currentScope, checking global for '" << key << "'" << std::endl;
    }
    
    // Ищем в глобальных зависимостях
    std::cout << ">>> IocContainer::resolve: acquiring globalMutex for '" << key << "'" << std::endl;
    std::lock_guard<std::recursive_mutex> lock(globalMutex_);
    std::cout << ">>> IocContainer::resolve: globalMutex acquired for '" << key << "'" << std::endl;
    
    auto it = globalDependencies_.find(key);
    if (it != globalDependencies_.end()) {
        std::cout << ">>> IocContainer::resolve: found in global, calling factory for '" << key << "'" << std::endl;
        auto result = it->second();
        std::cout << ">>> IocContainer::resolve: factory completed for '" << key << "'" << std::endl;
        return std::static_pointer_cast<T>(result);
    }
    
    std::cout << ">>> IocContainer::resolve: '" << key << "' not found anywhere" << std::endl;
    throw std::runtime_error("Dependency not found: " + key);
}
