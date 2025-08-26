# IoC Контейнер на C++17: Практическое руководство

## Содержание
- [1. Введение в IoC и DI](#1-введение-в-ioc-и-di)
- [2. Отличия C++ от C# при создании IoC](#2-отличия-c-от-c-при-создании-ioc)
- [3. Архитектурные подходы](#3-архитектурные-подходы)
- [4. Стратегии разрешения зависимостей](#4-стратегии-разрешения-зависимостей)
- [5. Управление памятью в C++ IoC](#5-управление-памятью-в-c-ioc)
- [6. Примеры реализации](#6-примеры-реализации)
- [7. Best Practices](#7-best-practices)
- [8. Заключение](#8-заключение)

---

## 1. Введение в IoC и DI

### 1.1. Основные концепции

**Inversion of Control (IoC)** - принцип проектирования, при котором управление созданием объектов и их жизненным циклом передается от самих объектов к внешнему контейнеру.

**Dependency Injection (DI)** - техника реализации IoC, при которой зависимости передаются в объект извне, а не создаются им самостоятельно.

### 1.2. Проблемы традиционного подхода

Из анализа лекций видим основную проблему:
```cpp
// Проблематичный код
class A {
public:
    A() {
        // Жесткая связанность - нарушает принципы SOLID
        b = new B(1, "abc");  
    }
private:
    B* b;
};
```

### 1.3. Преимущества IoC/DI

1. **Слабая связанность** - компоненты не зависят от конкретных реализаций
2. **Тестируемость** - легко подставлять моки и стабы
3. **Расширяемость** - можно легко добавлять новые реализации
4. **Переиспользуемость** - компоненты могут работать с разными зависимостями

---

## 2. Отличия C++ от C# при создании IoC

### 2.1. Управление памятью

#### C# (GC-based)
```csharp
// Автоматическое управление памятью
public class IocContainer 
{
    private Dictionary<string, Func<object[], object>> _dependencies;
    // GC автоматически освобождает память
}
```

#### C++ (Manual/RAII)
```cpp
#include <memory>
#include <unordered_map>
#include <functional>

// Используем умные указатели для автоматического управления памятью
class IocContainer {
private:
    std::unordered_map<std::string, std::function<std::shared_ptr<void>()>> dependencies_;
public:
    template<typename T>
    void registerFactory(const std::string& name, std::function<std::shared_ptr<T>()> factory) {
        dependencies_[name] = [factory]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(factory());
        };
    }
    
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) {
        auto it = dependencies_.find(name);
        if (it == dependencies_.end()) {
            throw std::runtime_error("Dependency not found: " + name);
        }
        return std::static_pointer_cast<T>(it->second());
    }
};
```

### 2.2. Система типов

#### C# - Runtime типизация
```csharp
public T Resolve<T>(string dependency, params object[] args) {
    return (T)_strategy(dependency, args);
}
```

#### C++ - Compile-time типизация
```cpp
template<typename T>
std::shared_ptr<T> resolve(const std::string& dependency) {
    // Type safety на этапе компиляции
    auto factory = getFactory<T>(dependency);
    return factory();
}
```

### 2.3. Обработка исключений

#### C# подход
```csharp
if ("Update Ioc Resolve Dependency Strategy" == dependency) {
    return new UpdateIocResolveDependencyStrategyCommand(/*...*/);
} else {
    throw new ArgumentException($"Dependency {dependency} is not found.");
}
```

#### C++ подход
```cpp
template<typename T>
std::shared_ptr<T> resolve(const std::string& dependency) {
    auto it = dependencies_.find(dependency);
    if (it == dependencies_.end()) {
        throw std::runtime_error("Could not find dependency: " + dependency + 
                                " for type: " + typeid(T).name());
    }
    return std::static_pointer_cast<T>(it->second());
}
```

---

## 3. Архитектурные подходы

### 3.1. Template-based подход (Compile-time)

**Преимущества:**
- Нулевые затраты времени выполнения
- Type safety на этапе компиляции
- Возможности инлайнинга

**Недостатки:**
- Весь код должен быть в заголовках
- Увеличение времени компиляции
- Сложность с динамической конфигурацией

```cpp
template<typename Logger, typename Database, typename Cache>
class Service {
private:
    Logger& logger_;
    Database& db_;
    Cache& cache_;
    
public:
    Service(Logger& logger, Database& db, Cache& cache)
        : logger_(logger), db_(db), cache_(cache) {}
        
    void doWork() {
        logger_.log("Starting work");
        auto data = db_.getData();
        cache_.store(data);
    }
};

// Использование
int main() {
    FileLogger logger;
    PostgresDB db;
    RedisCache cache;
    
    Service<FileLogger, PostgresDB, RedisCache> service(logger, db, cache);
    service.doWork();
}
```

### 3.2. Interface-based подход (Runtime)

**Преимущества:**
- Гибкость конфигурации времени выполнения
- Меньшее время компиляции
- Возможность загрузки плагинов

**Недостатки:**
- Накладные расходы на виртуальные вызовы
- Сложность управления временем жизни

```cpp
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual std::string getData() = 0;
};

class Service {
private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IDatabase> db_;
    
public:
    Service(std::shared_ptr<ILogger> logger, std::shared_ptr<IDatabase> db)
        : logger_(std::move(logger)), db_(std::move(db)) {}
        
    void doWork() {
        logger_->log("Starting work");
        auto data = db_->getData();
    }
};
```

### 3.3. Гибридный подход

Комбинирует преимущества обоих подходов:

```cpp
template<typename T>
class ServiceProvider {
public:
    virtual ~ServiceProvider() = default;
    virtual std::shared_ptr<T> create() = 0;
};

template<typename TInterface, typename TImpl>
class ConcreteProvider : public ServiceProvider<TInterface> {
public:
    std::shared_ptr<TInterface> create() override {
        return std::make_shared<TImpl>();
    }
};

class HybridContainer {
private:
    std::unordered_map<std::string, std::unique_ptr<ServiceProvider<void>>> providers_;
    
public:
    template<typename TInterface, typename TImpl>
    void registerType(const std::string& name) {
        providers_[name] = std::make_unique<ConcreteProvider<TInterface, TImpl>>();
    }
    
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) {
        auto it = providers_.find(name);
        if (it == providers_.end()) {
            throw std::runtime_error("Service not found: " + name);
        }
        auto provider = dynamic_cast<ServiceProvider<T>*>(it->second.get());
        return provider->create();
    }
};
```

---

## 4. Стратегии разрешения зависимостей

### 4.1. Базовые стратегии жизненного цикла

#### Transient (новый экземпляр каждый раз)
```cpp
template<typename T>
class TransientFactory {
public:
    std::shared_ptr<T> create() {
        return std::make_shared<T>();
    }
};
```

#### Singleton (один экземпляр на весь контейнер)
```cpp
template<typename T>
class SingletonFactory {
private:
    std::shared_ptr<T> instance_;
    std::mutex mutex_;
    
public:
    std::shared_ptr<T> create() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_) {
            instance_ = std::make_shared<T>();
        }
        return instance_;
    }
};
```

#### Object Pool (переиспользование экземпляров)
```cpp
template<typename T>
class PoolFactory {
private:
    std::queue<std::shared_ptr<T>> pool_;
    std::mutex mutex_;
    size_t maxSize_;
    
public:
    PoolFactory(size_t maxSize = 10) : maxSize_(maxSize) {}
    
    std::shared_ptr<T> create() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!pool_.empty()) {
            auto instance = pool_.front();
            pool_.pop();
            return instance;
        }
        return std::make_shared<T>();
    }
    
    void returnToPool(std::shared_ptr<T> instance) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pool_.size() < maxSize_) {
            pool_.push(instance);
        }
    }
};
```

### 4.2. Scoped Dependencies (по аналогии с C# примером)

```cpp
class Scope {
public:
    using FactoryFunc = std::function<std::shared_ptr<void>()>;
    
private:
    std::unordered_map<std::string, FactoryFunc> dependencies_;
    std::shared_ptr<Scope> parent_;
    
public:
    explicit Scope(std::shared_ptr<Scope> parent = nullptr) 
        : parent_(std::move(parent)) {}
    
    void registerDependency(const std::string& name, FactoryFunc factory) {
        dependencies_[name] = std::move(factory);
    }
    
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) {
        auto it = dependencies_.find(name);
        if (it != dependencies_.end()) {
            return std::static_pointer_cast<T>(it->second());
        }
        
        if (parent_) {
            return parent_->resolve<T>(name);
        }
        
        throw std::runtime_error("Dependency not found: " + name);
    }
};

// Thread-local текущий scope
thread_local std::shared_ptr<Scope> currentScope;

class ScopeGuard {
private:
    std::shared_ptr<Scope> previousScope_;
    
public:
    explicit ScopeGuard(std::shared_ptr<Scope> scope) 
        : previousScope_(currentScope) {
        currentScope = std::move(scope);
    }
    
    ~ScopeGuard() {
        currentScope = previousScope_;
    }
};
```

### 4.3. Constructor Injection

```cpp
template<typename T, typename... Args>
class ConstructorInjectionFactory {
private:
    IocContainer& container_;
    
public:
    explicit ConstructorInjectionFactory(IocContainer& container) 
        : container_(container) {}
    
    std::shared_ptr<T> create() {
        return std::make_shared<T>(container_.resolve<Args>()...);
    }
};

// Автоматическая регистрация с выводом типов
#define REGISTER_WITH_DEPS(container, T, ...) \
    container.registerFactory<T>(#T, \
        [&container]() { \
            return std::make_shared<ConstructorInjectionFactory<T, __VA_ARGS__>>(container).create(); \
        })
```

---

## 5. Управление памятью в C++ IoC

### 5.1. Стратегии управления временем жизни

#### Shared Ownership
```cpp
class Component {
private:
    std::shared_ptr<ILogger> logger_;  // Разделяемое владение
    
public:
    Component(std::shared_ptr<ILogger> logger) 
        : logger_(std::move(logger)) {}
};
```

#### Weak References (для избежания циклических ссылок)
```cpp
class Parent;
class Child {
private:
    std::weak_ptr<Parent> parent_;  // Слабая ссылка для избежания цикла
    
public:
    void setParent(std::shared_ptr<Parent> parent) {
        parent_ = parent;
    }
};
```

#### RAII-based Scope Management
```cpp
template<typename T>
class ScopedInstance {
private:
    std::shared_ptr<T> instance_;
    std::function<void(T*)> cleanup_;
    
public:
    template<typename CleanupFunc>
    ScopedInstance(std::shared_ptr<T> instance, CleanupFunc cleanup)
        : instance_(std::move(instance))
        , cleanup_([cleanup](T* ptr) { cleanup(ptr); }) {}
    
    ~ScopedInstance() {
        if (instance_) {
            cleanup_(instance_.get());
        }
    }
    
    T* get() const { return instance_.get(); }
    std::shared_ptr<T> shared() const { return instance_; }
};
```

### 5.2. Custom Allocators для IoC

```cpp
template<typename T>
class PoolAllocator {
private:
    std::vector<T> pool_;
    std::stack<T*> available_;
    
public:
    explicit PoolAllocator(size_t size) : pool_(size) {
        for (auto& item : pool_) {
            available_.push(&item);
        }
    }
    
    T* allocate() {
        if (available_.empty()) {
            throw std::bad_alloc();
        }
        T* ptr = available_.top();
        available_.pop();
        return ptr;
    }
    
    void deallocate(T* ptr) {
        available_.push(ptr);
    }
};

template<typename T>
class IoC_Container_WithCustomAlloc {
private:
    PoolAllocator<T> allocator_;
    
public:
    template<typename... Args>
    T* create(Args&&... args) {
        T* ptr = allocator_.allocate();
        new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }
    
    void destroy(T* ptr) {
        ptr->~T();
        allocator_.deallocate(ptr);
    }
};
```

---

## 6. Примеры реализации

### 6.1. Простейший IoC контейнер

```cpp
#include <memory>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <stdexcept>

class SimpleIoCContainer {
private:
    using FactoryFunction = std::function<std::shared_ptr<void>()>;
    std::unordered_map<std::type_index, FactoryFunction> factories_;
    
public:
    template<typename T, typename... Args>
    void registerType() {
        factories_[std::type_index(typeid(T))] = []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(std::make_shared<T>());
        };
    }
    
    template<typename T>
    void registerFactory(std::function<std::shared_ptr<T>()> factory) {
        factories_[std::type_index(typeid(T))] = [factory]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(factory());
        };
    }
    
    template<typename T>
    void registerInstance(std::shared_ptr<T> instance) {
        factories_[std::type_index(typeid(T))] = [instance]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(instance);
        };
    }
    
    template<typename T>
    std::shared_ptr<T> resolve() {
        auto it = factories_.find(std::type_index(typeid(T)));
        if (it == factories_.end()) {
            throw std::runtime_error(
                std::string("Type not registered: ") + typeid(T).name()
            );
        }
        return std::static_pointer_cast<T>(it->second());
    }
};
```

### 6.2. Расширенный IoC с поддержкой Scopes (адаптация C# примера)

```cpp
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <mutex>
#include <thread>

class AdvancedIoCContainer {
public:
    using FactoryFunction = std::function<std::shared_ptr<void>()>;
    
    enum class Lifetime {
        Transient,
        Singleton,
        Scoped
    };
    
private:
    struct ServiceDescriptor {
        FactoryFunction factory;
        Lifetime lifetime;
        std::shared_ptr<void> singletonInstance;
        std::mutex singletonMutex;
    };
    
    std::unordered_map<std::string, ServiceDescriptor> services_;
    std::unordered_map<std::thread::id, std::shared_ptr<void>> scopedInstances_;
    mutable std::mutex containerMutex_;
    
public:
    template<typename T>
    void registerTransient(const std::string& name, std::function<std::shared_ptr<T>()> factory) {
        std::lock_guard<std::mutex> lock(containerMutex_);
        services_[name] = {
            [factory]() -> std::shared_ptr<void> { 
                return std::static_pointer_cast<void>(factory()); 
            },
            Lifetime::Transient,
            nullptr
        };
    }
    
    template<typename T>
    void registerSingleton(const std::string& name, std::function<std::shared_ptr<T>()> factory) {
        std::lock_guard<std::mutex> lock(containerMutex_);
        services_[name] = {
            [factory]() -> std::shared_ptr<void> { 
                return std::static_pointer_cast<void>(factory()); 
            },
            Lifetime::Singleton,
            nullptr
        };
    }
    
    template<typename T>
    void registerScoped(const std::string& name, std::function<std::shared_ptr<T>()> factory) {
        std::lock_guard<std::mutex> lock(containerMutex_);
        services_[name] = {
            [factory]() -> std::shared_ptr<void> { 
                return std::static_pointer_cast<void>(factory()); 
            },
            Lifetime::Scoped,
            nullptr
        };
    }
    
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) {
        std::lock_guard<std::mutex> lock(containerMutex_);
        
        auto it = services_.find(name);
        if (it == services_.end()) {
            throw std::runtime_error("Service not found: " + name);
        }
        
        auto& descriptor = it->second;
        
        switch (descriptor.lifetime) {
            case Lifetime::Transient:
                return std::static_pointer_cast<T>(descriptor.factory());
                
            case Lifetime::Singleton:
            {
                std::lock_guard<std::mutex> singletonLock(descriptor.singletonMutex);
                if (!descriptor.singletonInstance) {
                    descriptor.singletonInstance = descriptor.factory();
                }
                return std::static_pointer_cast<T>(descriptor.singletonInstance);
            }
            
            case Lifetime::Scoped:
            {
                auto threadId = std::this_thread::get_id();
                auto scopedIt = scopedInstances_.find(threadId);
                if (scopedIt == scopedInstances_.end()) {
                    auto instance = descriptor.factory();
                    scopedInstances_[threadId] = instance;
                    return std::static_pointer_cast<T>(instance);
                }
                return std::static_pointer_cast<T>(scopedIt->second);
            }
        }
        
        return nullptr;
    }
    
    void clearScope() {
        std::lock_guard<std::mutex> lock(containerMutex_);
        scopedInstances_.erase(std::this_thread::get_id());
    }
};
```

### 6.3. IoC с командным паттерном (адаптация C# Ioc.cs)

```cpp
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

class IoC {
private:
    using StrategyFunction = std::function<std::shared_ptr<void>(const std::string&, const std::vector<std::shared_ptr<void>>&)>;
    static StrategyFunction strategy_;
    
public:
    template<typename T>
    static std::shared_ptr<T> resolve(const std::string& dependency, 
                                     const std::vector<std::shared_ptr<void>>& args = {}) {
        auto result = strategy_(dependency, args);
        auto typed_result = std::dynamic_pointer_cast<T>(result);
        if (!typed_result) {
            throw std::runtime_error("Cannot cast resolved dependency to requested type");
        }
        return typed_result;
    }
    
    static void updateStrategy(std::function<StrategyFunction(StrategyFunction)> updater) {
        strategy_ = updater(strategy_);
    }
};

// Определение статического члена
IoC::StrategyFunction IoC::strategy_ = [](const std::string& dependency, 
                                         const std::vector<std::shared_ptr<void>>& args) -> std::shared_ptr<void> {
    if (dependency == "Update IoC Resolve Dependency Strategy") {
        if (!args.empty()) {
            // Здесь должна быть логика обновления стратегии
            // Для примера возвращаем nullptr
        }
        throw std::invalid_argument("Update IoC strategy requires function argument");
    }
    throw std::invalid_argument("Dependency not found: " + dependency);
};

// Команда для обновления стратегии разрешения зависимостей
class UpdateIoCResolveDependencyStrategyCommand : public ICommand {
private:
    std::function<IoC::StrategyFunction(IoC::StrategyFunction)> updateStrategy_;
    
public:
    explicit UpdateIoCResolveDependencyStrategyCommand(
        std::function<IoC::StrategyFunction(IoC::StrategyFunction)> updater)
        : updateStrategy_(std::move(updater)) {}
    
    void execute() override {
        IoC::updateStrategy(updateStrategy_);
    }
};
```

### 6.4. Пример использования с многопоточностью

```cpp
#include <thread>
#include <vector>
#include <iostream>

// Интерфейсы
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void save(const std::string& data) = 0;
};

// Реализации
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[LOG] " << message << std::endl;
    }
};

class FileRepository : public IRepository {
public:
    void save(const std::string& data) override {
        std::cout << "[SAVE] " << data << std::endl;
    }
};

// Сервис, использующий зависимости
class BusinessService {
private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IRepository> repository_;
    
public:
    BusinessService(std::shared_ptr<ILogger> logger, std::shared_ptr<IRepository> repo)
        : logger_(std::move(logger)), repository_(std::move(repo)) {}
        
    void processData(const std::string& data) {
        logger_->log("Processing: " + data);
        repository_->save(data);
        logger_->log("Processing completed");
    }
};

// Демонстрация использования
int main() {
    AdvancedIoCContainer container;
    
    // Регистрация зависимостей
    container.registerSingleton<ILogger>("logger", []() {
        return std::make_shared<ConsoleLogger>();
    });
    
    container.registerScoped<IRepository>("repository", []() {
        return std::make_shared<FileRepository>();
    });
    
    container.registerTransient<BusinessService>("service", [&container]() {
        return std::make_shared<BusinessService>(
            container.resolve<ILogger>("logger"),
            container.resolve<IRepository>("repository")
        );
    });
    
    // Многопоточное использование
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&container, i]() {
            auto service = container.resolve<BusinessService>("service");
            service->processData("Data from thread " + std::to_string(i));
            
            // Очистка scoped зависимостей для текущего потока
            container.clearScope();
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}
```

---

## 7. Best Practices

### 7.1. Композиционный корень (Composition Root)

```cpp
class CompositionRoot {
public:
    static AdvancedIoCContainer setupContainer() {
        AdvancedIoCContainer container;
        
        // Регистрация инфраструктурных компонентов
        registerInfrastructure(container);
        
        // Регистрация бизнес-логики
        registerBusinessServices(container);
        
        // Регистрация контроллеров/презентеров
        registerControllers(container);
        
        return container;
    }
    
private:
    static void registerInfrastructure(AdvancedIoCContainer& container) {
        container.registerSingleton<ILogger>("logger", []() {
            return std::make_shared<ConsoleLogger>();
        });
        
        container.registerScoped<IDatabase>("database", []() {
            return std::make_shared<PostgresDatabase>("connection_string");
        });
    }
    
    static void registerBusinessServices(AdvancedIoCContainer& container) {
        container.registerTransient<UserService>("userService", [&container]() {
            return std::make_shared<UserService>(
                container.resolve<ILogger>("logger"),
                container.resolve<IUserRepository>("userRepository")
            );
        });
    }
    
    static void registerControllers(AdvancedIoCContainer& container) {
        container.registerTransient<UserController>("userController", [&container]() {
            return std::make_shared<UserController>(
                container.resolve<UserService>("userService")
            );
        });
    }
};
```

### 7.2. Обработка ошибок

```cpp
class ContainerException : public std::exception {
private:
    std::string message_;
    
public:
    explicit ContainerException(const std::string& message) 
        : message_(message) {}
        
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class CircularDependencyException : public ContainerException {
public:
    explicit CircularDependencyException(const std::string& dependency)
        : ContainerException("Circular dependency detected: " + dependency) {}
};

// Детектор циклических зависимостей
class DependencyTracker {
private:
    std::unordered_set<std::string> currentlyResolving_;
    
public:
    class Guard {
    private:
        DependencyTracker& tracker_;
        std::string dependency_;
        
    public:
        Guard(DependencyTracker& tracker, const std::string& dependency)
            : tracker_(tracker), dependency_(dependency) {
            if (!tracker_.currentlyResolving_.insert(dependency).second) {
                throw CircularDependencyException(dependency);
            }
        }
        
        ~Guard() {
            tracker_.currentlyResolving_.erase(dependency_);
        }
    };
    
    Guard track(const std::string& dependency) {
        return Guard(*this, dependency);
    }
};
```

### 7.3. Performance Optimization

#### Compile-time строковые ключи
```cpp
#include <string_view>

template<size_t N>
struct CompileTimeString {
    constexpr CompileTimeString(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
    
    constexpr std::string_view view() const {
        return std::string_view(value, N - 1);
    }
};

template<CompileTimeString Key>
class TypedService {
public:
    static constexpr std::string_view key() {
        return Key.view();
    }
};

// Использование
using LoggerService = TypedService<"logger">;
using DatabaseService = TypedService<"database">;
```

#### Lock-free регистрация (только для чтения после инициализации)
```cpp
class ReadOnlyContainer {
private:
    std::unordered_map<std::string, FactoryFunction> factories_;
    std::atomic<bool> initialized_{false};
    
public:
    void finalize() {
        initialized_.store(true, std::memory_order_release);
    }
    
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) {
        if (!initialized_.load(std::memory_order_acquire)) {
            throw std::runtime_error("Container not initialized");
        }
        
        // Теперь можно обращаться без блокировок
        auto it = factories_.find(name);
        if (it == factories_.end()) {
            throw std::runtime_error("Service not found: " + name);
        }
        return std::static_pointer_cast<T>(it->second());
    }
};
```

### 7.4. Интеграция с C++20 Concepts

```cpp
#include <concepts>

template<typename T>
concept Injectable = requires(T t) {
    typename T::dependencies;  // Тип должен определять свои зависимости
};

template<typename T>
concept Resolvable = requires {
    std::is_default_constructible_v<T> || Injectable<T>;
};

template<Resolvable T>
class AutoRegisteringContainer {
public:
    std::shared_ptr<T> resolve() {
        if constexpr (std::is_default_constructible_v<T>) {
            return std::make_shared<T>();
        } else {
            return resolveWithDependencies<T>();
        }
    }
    
private:
    template<Injectable T>
    std::shared_ptr<T> resolveWithDependencies() {
        // Автоматическое разрешение зависимостей на основе T::dependencies
        return createWithDependencies<T, typename T::dependencies>();
    }
};
```

### 7.5. Debugging и Profiling

```cpp
class DiagnosticContainer : public AdvancedIoCContainer {
private:
    struct ResolutionInfo {
        std::chrono::high_resolution_clock::time_point timestamp;
        std::chrono::milliseconds duration;
        std::string serviceName;
        std::thread::id threadId;
    };
    
    std::vector<ResolutionInfo> resolutionHistory_;
    mutable std::mutex historyMutex_;
    
public:
    template<typename T>
    std::shared_ptr<T> resolve(const std::string& name) override {
        auto start = std::chrono::high_resolution_clock::now();
        
        auto result = AdvancedIoCContainer::resolve<T>(name);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        {
            std::lock_guard<std::mutex> lock(historyMutex_);
            resolutionHistory_.push_back({
                start,
                duration,
                name,
                std::this_thread::get_id()
            });
        }
        
        return result;
    }
    
    void printDiagnostics() const {
        std::lock_guard<std::mutex> lock(historyMutex_);
        std::cout << "Resolution History:" << std::endl;
        for (const auto& info : resolutionHistory_) {
            std::cout << "Service: " << info.serviceName 
                      << ", Duration: " << info.duration.count() << "ms"
                      << ", Thread: " << info.threadId << std::endl;
        }
    }
};
```

---

## 8. Заключение

### 8.1. Сравнение подходов

| Критерий | Template-based | Interface-based | Гибридный |
|----------|----------------|-----------------|-----------|
| Performance | Отлично | Хорошо | Хорошо |
| Гибкость | Ограниченная | Отличная | Отличная |
| Type Safety | Compile-time | Runtime | Runtime |
| Время компиляции | Медленное | Быстрое | Среднее |
| Размер бинарника | Большой | Средний | Средний |

### 8.2. Рекомендации по выбору

1. **Template-based подход** - для высокопроизводительных систем с фиксированной конфигурацией
2. **Interface-based подход** - для систем с динамической конфигурацией и плагинами  
3. **Гибридный подход** - для большинства enterprise приложений

### 8.3. Ключевые принципы реализации C++ IoC

1. **Используйте RAII** для автоматического управления ресурсами
2. **Предпочитайте `std::shared_ptr`** для разделяемого владения
3. **Избегайте циклических зависимостей** или используйте `std::weak_ptr`
4. **Планируйте стратегию многопоточности** заранее
5. **Минимизируйте время компиляции** разделением интерфейсов и реализаций
6. **Используйте статическую типизацию** где возможно для обнаружения ошибок на этапе компиляции
7. **Документируйте зависимости** для облегчения сопровождения

### 8.4. Интеграция с существующим кодом

Для интеграции IoC в legacy код:

```cpp
// Legacy адаптер
class LegacyServiceAdapter : public IModernService {
private:
    LegacyService* legacyService_;
    
public:
    explicit LegacyServiceAdapter(LegacyService* service) 
        : legacyService_(service) {}
    
    void modernMethod() override {
        legacyService_->oldMethod();
    }
};

// Регистрация адаптера
container.registerFactory<IModernService>("modernService", [&legacyInstance]() {
    return std::make_shared<LegacyServiceAdapter>(&legacyInstance);
});
```

Этот конспект предоставляет полное руководство по созданию IoC контейнера на C++17 с учетом особенностей языка и лучших практик индустрии.