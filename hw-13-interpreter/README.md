# Домашнее задание 13: Система команд для космических кораблей

## Интерпретатор приказов (паттерн Interpreter)

**Автор:** Anton Tobolkin  
**Курс:** Архитектура и паттерны проектирования  
**Максимальная оценка:** 10 баллов  
**Минимальная оценка для зачета:** 7 баллов

---

## 📋 Требования к домашнему заданию

### Цель
Научить игровые объекты реагировать на действия игроков, используя паттерн Интерпретатор.

### Формат приказа
```json
{
  "id": "ид объекта, которому адресован приказ",
  "action": "действие, которое необходимо выполнить",
  // специфичные параметры для данного приказа
}
```

### Пример приказа
```json
{
  "id": "548",
  "action": "StartMove",
  "initialVelocity": 2
}
```

---

## 🎯 Критерии оценки

| Критерий | Баллы | Описание |
|----------|-------|----------|
| Задача сдана на проверку | 1 | MR создан, код загружен |
| Оформлен MR/PR | 1 | Правильное оформление |
| CI | 1 | Настроен и работает ✅ (уже сделано) |
| **Интерпретатор приказов** | **1-3** | См. детали ниже |
| Тесты интерпретатора | 1 | Unit-тесты |
| **Защита от чужих приказов** | **1-2** | См. детали ниже |
| Тесты защиты | 1 | Unit-тесты |
| **ИТОГО** | **10** | |

### Детализация баллов за интерпретатор

| Баллы | Условие |
|-------|---------|
| 1 | Можно обработать приказы: StartMove, StopMove, Fire |
| 2 | Преподаватель не может привести пример приказа, требующего изменения кода |
| 3 | Можно обрабатывать не только приказы игровым объектам, но и любые другие |

### Детализация баллов за защиту

| Баллы | Условие |
|-------|---------|
| 1 | Защита реализована любым способом |
| 2 | Защита через отдельные скоупы для каждого игрока |

---

## 🏗️ Архитектура проекта

### Изменения в common-библиотеке

Для унификации работы с key-value объектами вводим базовый интерфейс `IUObject`:

```cpp
// common/include/IUObject.hpp - базовый универсальный объект
class IUObject {
public:
    virtual ~IUObject() = default;
    virtual std::any getProperty(const std::string& key) const = 0;
    virtual void setProperty(const std::string& key, const std::any& value) = 0;
};

// common/include/IGameObject.hpp - наследник для игровых объектов
#include "IUObject.hpp"

class IGameObject : public IUObject {
public:
    ~IGameObject() override = default;
    // В будущем можно добавить игро-специфичные методы
};
```

**Преимущества:**
- Минимум изменений в существующем коде (GameObject уже реализует эти методы)
- `IUObject` используется для `parameters` в `Order` — доменная модель без JSON
- Консистентность — везде один подход к key-value хранению

### Структура директорий

```
hw-13-interpreter/
├── CMakeLists.txt
├── README.md                          # Этот файл
├── docs/
│   └── hw-13-task.md                  # Исходное задание
├── include/
│   ├── Order.hpp                      # Доменная модель приказа (без JSON!)
│   ├── IOrderInterpreter.hpp          # Интерфейс интерпретатора
│   ├── IOrderParser.hpp               # Интерфейс парсера приказов
│   ├── OrderInterpreter.hpp           # Базовая реализация
│   ├── SecureOrderInterpreter.hpp     # Защищенная реализация
│   ├── JsonOrderParser.hpp            # Адаптер: JSON → Order
│   └── commands/
│       ├── StartMoveCommand.hpp       # Команда начала движения
│       ├── StopMoveCommand.hpp        # Команда остановки
│       ├── FireCommand.hpp            # Команда выстрела
│       ├── RotateCommand.hpp          # Команда поворота
│       └── SetVelocityCommand.hpp     # Команда установки скорости
├── src/
│   ├── OrderInterpreter.cpp
│   ├── SecureOrderInterpreter.cpp
│   ├── JsonOrderParser.cpp
│   └── commands/
│       ├── StartMoveCommand.cpp
│       ├── StopMoveCommand.cpp
│       ├── FireCommand.cpp
│       ├── RotateCommand.cpp
│       └── SetVelocityCommand.cpp
└── tests/
    ├── OrderInterpreterTest.cpp       # Тесты интерпретатора
    ├── SecureOrderInterpreterTest.cpp # Тесты защиты
    ├── JsonOrderParserTest.cpp        # Тесты парсера
    └── CommandsTest.cpp               # Тесты команд
```

---

## 📐 Диаграммы

### Диаграмма классов

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                               IUObject                                       │
│  <<interface>>                                                               │
├─────────────────────────────────────────────────────────────────────────────┤
│  + getProperty(key: string): any                                            │
│  + setProperty(key: string, value: any): void                               │
└─────────────────────────────────────────────────────────────────────────────┘
                                       △
                                       │
           ┌───────────────────────────┼───────────────────────────┐
           │                           │                           │
┌─────────────────────────┐  ┌─────────────────────────┐  ┌─────────────────┐
│     IGameObject         │  │     UObject (impl)      │  │  Order.params   │
│  <<interface>>          │  ├─────────────────────────┤  │  (использует)   │
├─────────────────────────┤  │  - props_: map<any>     │  └─────────────────┘
│  (игро-специфичные)     │  └─────────────────────────┘
└─────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                                  Order                                       │
│  <<struct>>                                                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│  + objectId: string                    # ID объекта-получателя              │
│  + action: string                      # Действие для выполнения            │
│  + parameters: shared_ptr<IUObject>    # Параметры (без JSON!)              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                            IOrderParser                                      │
│  <<interface>>                                                               │
├─────────────────────────────────────────────────────────────────────────────┤
│  + parse(input: string): Order                                              │
└─────────────────────────────────────────────────────────────────────────────┘
                                       △
                                       │
┌─────────────────────────────────────────────────────────────────────────────┐
│                           JsonOrderParser                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│  + parse(jsonString: string): Order   # JSON → Order (адаптер)              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                          IOrderInterpreter                                   │
│  <<interface>>                                                               │
├─────────────────────────────────────────────────────────────────────────────┤
│  + interpret(order: Order): void                                            │
└─────────────────────────────────────────────────────────────────────────────┘
                                    △
                                    │
                    ┌───────────────┴───────────────┐
                    │                               │
┌───────────────────────────────┐   ┌───────────────────────────────────┐
│      OrderInterpreter         │   │    SecureOrderInterpreter          │
├───────────────────────────────┤   ├───────────────────────────────────┤
│  - container_: IocContainer&  │   │  - container_: IocContainer&       │
├───────────────────────────────┤   │  - playerId_: string               │
│  + interpret(order): void     │   ├───────────────────────────────────┤
│  - resolveObject(id): IUObj   │   │  + interpret(order): void          │
│  - createCommand(action,obj,  │   │  - setPlayerScope(): void          │
│      params): ICommand        │   │  - validateAccess(objectId): bool  │
└───────────────────────────────┘   └───────────────────────────────────┘
                │                                    │
                │ использует                         │ использует
                ▼                                    ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           IocContainer                                       │
├─────────────────────────────────────────────────────────────────────────────┤
│  + resolve<T>(key, args): shared_ptr<T>                                     │
│  + createScope(id): Scope                                                    │
│  + setCurrentScope(id): void                                                 │
└─────────────────────────────────────────────────────────────────────────────┘
                │
                │ создает
                ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            ICommand                                          │
│  <<interface>>                                                               │
├─────────────────────────────────────────────────────────────────────────────┤
│  + execute(): void                                                           │
└─────────────────────────────────────────────────────────────────────────────┘
                                    △
                                    │
        ┌───────────────┬───────────┼───────────┬───────────────┐
        │               │           │           │               │
┌───────────────┐ ┌───────────────┐ ┌─────────────┐ ┌───────────────┐
│StartMoveCmd   │ │StopMoveCmd    │ │ FireCommand │ │RotateCommand  │
├───────────────┤ ├───────────────┤ ├─────────────┤ ├───────────────┤
│- obj_: IUObj  │ │- obj_: IUObj  │ │- obj_       │ │- obj_         │
│- velocity_    │ ├───────────────┤ │- targetId_  │ │- angle_       │
├───────────────┤ │+ execute()    │ ├─────────────┤ ├───────────────┤
│+ execute()    │ └───────────────┘ │+ execute()  │ │+ execute()    │
└───────────────┘                   └─────────────┘ └───────────────┘
```

### Диаграмма последовательности (полный flow)

```
┌────────┐   ┌────────────────┐   ┌────────────────────┐   ┌──────────────┐   ┌──────────┐
│ Client │   │ JsonOrderParser│   │ OrderInterpreter   │   │ IocContainer │   │ ICommand │
└───┬────┘   └───────┬────────┘   └─────────┬──────────┘   └──────┬───────┘   └────┬─────┘
    │                │                      │                     │                │
    │ parse(jsonStr) │                      │                     │                │
    │───────────────>│                      │                     │                │
    │                │                      │                     │                │
    │     Order      │                      │                     │                │
    │<───────────────│                      │                     │                │
    │                │                      │                     │                │
    │ interpret(order)                      │                     │                │
    │──────────────────────────────────────>│                     │                │
    │                │                      │                     │                │
    │                │                      │ resolve("Objects.548")               │
    │                │                      │────────────────────>│                │
    │                │                      │                     │                │
    │                │                      │ <IUObject>          │                │
    │                │                      │<────────────────────│                │
    │                │                      │                     │                │
    │                │                      │ resolve("Commands.StartMove", args)  │
    │                │                      │────────────────────>│                │
    │                │                      │                     │                │
    │                │                      │ <ICommand>          │                │
    │                │                      │<────────────────────│                │
    │                │                      │                     │                │
    │                │                      │                     │     execute()  │
    │                │                      │────────────────────────────────────>│
    │                │                      │                     │                │
    │     OK         │                      │                     │                │
    │<──────────────────────────────────────│                     │                │
```

### Диаграмма преобразования данных

```
JSON (входные данные от клиента)
        │
        │  {"id": "548", "action": "StartMove", "initialVelocity": 2}
        │
        ▼
┌───────────────────────────────┐
│      JsonOrderParser          │  ← Адаптер (единственное место с JSON)
│      (слой инфраструктуры)    │
└───────────────────────────────┘
        │
        │  Order {
        │    objectId: "548",
        │    action: "StartMove",
        │    parameters: IUObject* { "initialVelocity" → 2 }
        │  }
        │
        ▼
┌───────────────────────────────┐
│      OrderInterpreter         │  ← Работает только с абстракциями
│      (слой бизнес-логики)     │
└───────────────────────────────┘
        │
        │  resolve → IUObject (игровой объект)
        │  resolve → ICommand (команда для действия)
        │
        ▼
┌───────────────────────────────┐
│      ICommand::execute()      │
└───────────────────────────────┘
```

### Диаграмма скоупов (защита)

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              ROOT SCOPE                                      │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │  Global Dependencies:                                                  │   │
│  │    - Commands.StartMove → StartMoveCommandFactory                     │   │
│  │    - Commands.StopMove → StopMoveCommandFactory                       │   │
│  │    - Commands.Fire → FireCommandFactory                               │   │
│  │    - Commands.Rotate → RotateCommandFactory                           │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                              │
│  ┌──────────────────────────────────┐  ┌──────────────────────────────────┐ │
│  │         Player.Alice Scope       │  │         Player.Bob Scope         │ │
│  │  ┌────────────────────────────┐  │  │  ┌────────────────────────────┐  │ │
│  │  │  Objects.ship_001 → Ship1  │  │  │  │  Objects.ship_003 → Ship3  │  │ │
│  │  │  Objects.ship_002 → Ship2  │  │  │  │  Objects.ship_004 → Ship4  │  │ │
│  │  └────────────────────────────┘  │  │  └────────────────────────────┘  │ │
│  └──────────────────────────────────┘  └──────────────────────────────────┘ │
│                                                                              │
│  Alice может управлять только ship_001 и ship_002                           │
│  Bob может управлять только ship_003 и ship_004                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 📝 Псевдокод

### 1. Доменная модель Order (без JSON!)

```cpp
// include/Order.hpp

#pragma once

#include "IUObject.hpp"
#include <string>
#include <memory>

/**
 * @brief Доменная модель приказа
 * 
 * Не зависит от формата сериализации (JSON, XML, Protobuf и т.д.)
 * Параметры хранятся как IUObject для унифицированного доступа.
 * 
 * @author Anton Tobolkin
 */
struct Order {
    std::string objectId;                      ///< ID объекта-получателя
    std::string action;                        ///< Действие для выполнения
    std::shared_ptr<IUObject> parameters;      ///< Параметры действия
    
    /**
     * @brief Проверяет валидность приказа
     * @return true если приказ содержит обязательные поля
     */
    bool isValid() const {
        return !objectId.empty() && !action.empty();
    }
};
```

### 2. Интерфейс парсера приказов

```cpp
// include/IOrderParser.hpp

#pragma once

#include "Order.hpp"
#include <string>

/**
 * @brief Интерфейс парсера приказов
 * 
 * Абстрагирует формат входных данных от доменной модели.
 * Позволяет поддерживать разные форматы (JSON, XML, бинарный и т.д.)
 * 
 * @author Anton Tobolkin
 */
class IOrderParser {
public:
    virtual ~IOrderParser() = default;
    
    /**
     * @brief Парсит входные данные в Order
     * @param input Строка с данными приказа
     * @return Order Распарсенный приказ
     * @throws std::runtime_error при ошибке парсинга
     */
    virtual Order parse(const std::string& input) = 0;
};
```

### 3. JSON парсер (адаптер)

```cpp
// include/JsonOrderParser.hpp

#pragma once

#include "IOrderParser.hpp"
#include <nlohmann/json.hpp>

/**
 * @brief Парсер приказов из JSON формата
 * 
 * Единственное место в системе, знающее о JSON.
 * Преобразует JSON в доменную модель Order.
 * 
 * @author Anton Tobolkin
 */
class JsonOrderParser : public IOrderParser {
public:
    /**
     * @brief Парсит JSON-строку в Order
     * 
     * Формат JSON:
     * {
     *   "id": "object_id",
     *   "action": "ActionName",
     *   "param1": value1,
     *   ...
     * }
     */
    Order parse(const std::string& jsonString) override;
    
private:
    /**
     * @brief Конвертирует JSON-параметры в IUObject
     */
    std::shared_ptr<IUObject> convertParameters(const nlohmann::json& json);
};
```

```cpp
// src/JsonOrderParser.cpp

#include "JsonOrderParser.hpp"
#include "UObject.hpp"  // Конкретная реализация IUObject

Order JsonOrderParser::parse(const std::string& jsonString)
{
    nlohmann::json json = nlohmann::json::parse(jsonString);
    
    // Валидация обязательных полей
    if (!json.contains("id")) {
        throw std::runtime_error("Order must contain 'id' field");
    }
    if (!json.contains("action")) {
        throw std::runtime_error("Order must contain 'action' field");
    }
    
    Order order;
    order.objectId = json["id"].get<std::string>();
    order.action = json["action"].get<std::string>();
    order.parameters = convertParameters(json);
    
    return order;
}

std::shared_ptr<IUObject> JsonOrderParser::convertParameters(const nlohmann::json& json)
{
    auto params = std::make_shared<UObject>();
    
    // Копируем все поля кроме id и action
    for (auto& [key, value] : json.items()) {
        if (key == "id" || key == "action") continue;
        
        if (value.is_number_integer()) {
            params->setProperty(key, value.get<int>());
        } else if (value.is_number_float()) {
            params->setProperty(key, value.get<double>());
        } else if (value.is_string()) {
            params->setProperty(key, value.get<std::string>());
        } else if (value.is_boolean()) {
            params->setProperty(key, value.get<bool>());
        }
        // Сложные типы можно добавить при необходимости
    }
    
    return params;
}
```

### 4. Интерфейс интерпретатора

```cpp
// include/IOrderInterpreter.hpp

#pragma once

#include "Order.hpp"

/**
 * @brief Интерфейс интерпретатора приказов
 * @author Anton Tobolkin
 */
class IOrderInterpreter {
public:
    virtual ~IOrderInterpreter() = default;
    
    /**
     * @brief Интерпретирует и выполняет приказ
     * @param order Доменная модель приказа
     * @throws std::runtime_error при ошибке интерпретации
     */
    virtual void interpret(const Order& order) = 0;
};
```

### 5. Базовая реализация интерпретатора

```cpp
// include/OrderInterpreter.hpp

#pragma once

#include "IOrderInterpreter.hpp"
#include "IocContainer.hpp"
#include "ICommand.hpp"
#include "IUObject.hpp"

/**
 * @brief Интерпретатор приказов для игровых объектов
 * 
 * Использует IoC для динамического создания команд,
 * что позволяет добавлять новые команды без изменения кода.
 * 
 * @author Anton Tobolkin
 */
class OrderInterpreter : public IOrderInterpreter {
public:
    /**
     * @brief Конструктор
     * @param container Ссылка на IoC контейнер
     */
    explicit OrderInterpreter(IocContainer& container);
    
    /**
     * @brief Интерпретирует приказ
     */
    void interpret(const Order& order) override;

protected:
    /**
     * @brief Разрешает объект по ID
     * @param objectId Идентификатор объекта
     * @return Указатель на IUObject
     */
    virtual std::shared_ptr<IUObject> resolveObject(const std::string& objectId);
    
    /**
     * @brief Создает команду для действия
     * @param action Название действия
     * @param object Целевой объект
     * @param params Параметры команды
     * @return Указатель на команду
     */
    virtual std::shared_ptr<ICommand> createCommand(
        const std::string& action,
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);

    IocContainer& container_;
};
```

### 6. Реализация интерпретатора

```cpp
// src/OrderInterpreter.cpp

#include "OrderInterpreter.hpp"
#include <stdexcept>

OrderInterpreter::OrderInterpreter(IocContainer& container)
    : container_(container)
{
}

void OrderInterpreter::interpret(const Order& order)
{
    // Валидация
    if (!order.isValid()) {
        throw std::runtime_error("Invalid order: missing id or action");
    }
    
    // Разрешение объекта через IoC
    auto object = resolveObject(order.objectId);
    
    // Создание команды через IoC
    auto command = createCommand(order.action, object, order.parameters);
    
    // Выполнение команды
    command->execute();
}

std::shared_ptr<IUObject> OrderInterpreter::resolveObject(const std::string& objectId)
{
    try {
        return container_.resolve<IUObject>("Objects." + objectId);
    } catch (const std::exception& e) {
        throw std::runtime_error("Object not found: " + objectId);
    }
}

std::shared_ptr<ICommand> OrderInterpreter::createCommand(
    const std::string& action,
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
{
    try {
        // Формируем аргументы для фабрики команд
        std::vector<std::shared_ptr<void>> args = {
            std::static_pointer_cast<void>(object),
            std::static_pointer_cast<void>(params)
        };
        
        return container_.resolve<ICommand>("Commands." + action, args);
    } catch (const std::exception& e) {
        throw std::runtime_error("Unknown action: " + action);
    }
}
```

### 7. Защищенный интерпретатор (через скоупы)

```cpp
// include/SecureOrderInterpreter.hpp

#pragma once

#include "OrderInterpreter.hpp"

/**
 * @brief Защищенный интерпретатор с проверкой доступа через скоупы
 * 
 * Каждый игрок работает в своем изолированном скоупе.
 * Объекты регистрируются в скоупе владельца.
 * При попытке управления чужим объектом - исключение.
 * 
 * @author Anton Tobolkin
 */
class SecureOrderInterpreter : public OrderInterpreter {
public:
    /**
     * @brief Конструктор
     * @param container IoC контейнер
     * @param playerId Идентификатор игрока
     */
    SecureOrderInterpreter(IocContainer& container, const std::string& playerId);
    
    /**
     * @brief Интерпретирует приказ с проверкой доступа
     */
    void interpret(const Order& order) override;

protected:
    /**
     * @brief Разрешает объект в скоупе игрока
     */
    std::shared_ptr<IUObject> resolveObject(const std::string& objectId) override;

private:
    /**
     * @brief Устанавливает скоуп текущего игрока
     */
    void setPlayerScope();
    
    /**
     * @brief Создает скоуп для игрока если не существует
     */
    void ensurePlayerScopeExists();

    std::string playerId_;
};
```

### 8. Реализация защищенного интерпретатора

```cpp
// src/SecureOrderInterpreter.cpp

#include "SecureOrderInterpreter.hpp"

SecureOrderInterpreter::SecureOrderInterpreter(
    IocContainer& container, 
    const std::string& playerId)
    : OrderInterpreter(container)
    , playerId_(playerId)
{
    ensurePlayerScopeExists();
}

void SecureOrderInterpreter::interpret(const Order& order)
{
    // Устанавливаем скоуп текущего игрока
    setPlayerScope();
    
    // Вызываем базовую реализацию
    // Она попытается разрешить объект в текущем скоупе
    OrderInterpreter::interpret(order);
}

std::shared_ptr<IUObject> SecureOrderInterpreter::resolveObject(
    const std::string& objectId)
{
    try {
        // Попытка разрешить в скоупе игрока
        return container_.resolve<IUObject>("Objects." + objectId);
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Access denied: object '" + objectId + 
            "' does not belong to player '" + playerId_ + "'"
        );
    }
}

void SecureOrderInterpreter::setPlayerScope()
{
    auto scopeId = std::make_shared<std::string>("Player." + playerId_);
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    auto setCommand = IoC::resolve<ICommand>("Scopes.Current", args);
    setCommand->execute();
}

void SecureOrderInterpreter::ensurePlayerScopeExists()
{
    auto scopeId = std::make_shared<std::string>("Player." + playerId_);
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    try {
        auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
        createCommand->execute();
    } catch (...) {
        // Скоуп уже существует - это нормально
    }
}
```

### 9. Пример команды StartMove

```cpp
// include/commands/StartMoveCommand.hpp

#pragma once

#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>

/**
 * @brief Команда начала движения
 * @author Anton Tobolkin
 */
class StartMoveCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param object Объект для управления
     * @param params Параметры (должен содержать initialVelocity)
     */
    StartMoveCommand(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);
    
    void execute() override;

private:
    std::shared_ptr<IUObject> object_;
    int initialVelocity_;
};

// src/commands/StartMoveCommand.cpp

#include "commands/StartMoveCommand.hpp"

StartMoveCommand::StartMoveCommand(
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
    : object_(object)
{
    // Получаем параметры через IUObject интерфейс
    try {
        initialVelocity_ = std::any_cast<int>(params->getProperty("initialVelocity"));
    } catch (const std::exception& e) {
        throw std::runtime_error("StartMove requires 'initialVelocity' parameter");
    }
}

void StartMoveCommand::execute()
{
    // Устанавливаем скорость через свойство объекта
    object_->setProperty("velocity", initialVelocity_);
    
    // Можно также добавить объект в очередь движущихся объектов
    // или установить флаг isMoving
    object_->setProperty("isMoving", true);
}
```

### 10. Регистрация команд в IoC

```cpp
// В коде инициализации приложения

void registerCommands(IocContainer& container)
{
    // StartMove
    container.registerGlobalDependency(
        "Commands.StartMove",
        [](std::vector<std::shared_ptr<void>>& args) {
            auto object = std::static_pointer_cast<IUObject>(args[0]);
            auto params = std::static_pointer_cast<IUObject>(args[1]);
            return std::make_shared<StartMoveCommand>(object, params);
        }
    );
    
    // StopMove
    container.registerGlobalDependency(
        "Commands.StopMove",
        [](std::vector<std::shared_ptr<void>>& args) {
            auto object = std::static_pointer_cast<IUObject>(args[0]);
            return std::make_shared<StopMoveCommand>(object);
        }
    );
    
    // Fire
    container.registerGlobalDependency(
        "Commands.Fire",
        [](std::vector<std::shared_ptr<void>>& args) {
            auto object = std::static_pointer_cast<IUObject>(args[0]);
            auto params = std::static_pointer_cast<IUObject>(args[1]);
            return std::make_shared<FireCommand>(object, params);
        }
    );
    
    // Rotate
    container.registerGlobalDependency(
        "Commands.Rotate",
        [](std::vector<std::shared_ptr<void>>& args) {
            auto object = std::static_pointer_cast<IUObject>(args[0]);
            auto params = std::static_pointer_cast<IUObject>(args[1]);
            return std::make_shared<RotateCommand>(object, params);
        }
    );
}
```

---

## 🧪 Тесты

### Тесты парсера (JsonOrderParser)

```cpp
// tests/JsonOrderParserTest.cpp

#include <gtest/gtest.h>
#include "JsonOrderParser.hpp"

class JsonOrderParserTest : public ::testing::Test {
protected:
    JsonOrderParser parser_;
};

TEST_F(JsonOrderParserTest, ParseValidOrder_Success)
{
    std::string json = R"({
        "id": "ship_001",
        "action": "StartMove",
        "initialVelocity": 5
    })";
    
    Order order = parser_.parse(json);
    
    EXPECT_EQ(order.objectId, "ship_001");
    EXPECT_EQ(order.action, "StartMove");
    EXPECT_TRUE(order.isValid());
    
    auto velocity = std::any_cast<int>(order.parameters->getProperty("initialVelocity"));
    EXPECT_EQ(velocity, 5);
}

TEST_F(JsonOrderParserTest, ParseMissingId_ThrowsException)
{
    std::string json = R"({
        "action": "StartMove",
        "initialVelocity": 5
    })";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

TEST_F(JsonOrderParserTest, ParseMissingAction_ThrowsException)
{
    std::string json = R"({
        "id": "ship_001",
        "initialVelocity": 5
    })";
    
    EXPECT_THROW(parser_.parse(json), std::runtime_error);
}

TEST_F(JsonOrderParserTest, ParseInvalidJson_ThrowsException)
{
    std::string json = "not a valid json";
    
    EXPECT_THROW(parser_.parse(json), std::exception);
}

TEST_F(JsonOrderParserTest, ParseMultipleParams_AllExtracted)
{
    std::string json = R"({
        "id": "ship_001",
        "action": "Fire",
        "targetId": "enemy_001",
        "power": 100,
        "angle": 45.5
    })";
    
    Order order = parser_.parse(json);
    
    auto targetId = std::any_cast<std::string>(order.parameters->getProperty("targetId"));
    auto power = std::any_cast<int>(order.parameters->getProperty("power"));
    auto angle = std::any_cast<double>(order.parameters->getProperty("angle"));
    
    EXPECT_EQ(targetId, "enemy_001");
    EXPECT_EQ(power, 100);
    EXPECT_DOUBLE_EQ(angle, 45.5);
}
```

### Тесты интерпретатора

```cpp
// tests/OrderInterpreterTest.cpp

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "OrderInterpreter.hpp"
#include "Order.hpp"
#include "IUObject.hpp"
#include "ICommand.hpp"

using ::testing::_;
using ::testing::Return;

// Mock объекты
class MockUObject : public IUObject {
public:
    MOCK_METHOD(std::any, getProperty, (const std::string&), (const, override));
    MOCK_METHOD(void, setProperty, (const std::string&, const std::any&), (override));
};

class MockCommand : public ICommand {
public:
    MOCK_METHOD(void, execute, (), (override));
};

// Хелпер для создания Order
Order createTestOrder(
    const std::string& objectId,
    const std::string& action,
    std::shared_ptr<IUObject> params = nullptr)
{
    Order order;
    order.objectId = objectId;
    order.action = action;
    order.parameters = params ? params : std::make_shared<MockUObject>();
    return order;
}

class OrderInterpreterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка IoC контейнера для тестов
        setupIoC();
    }
    
    void setupIoC() {
        // Регистрация тестовых зависимостей
    }
};

TEST_F(OrderInterpreterTest, InterpretStartMove_Success)
{
    auto params = std::make_shared<MockUObject>();
    EXPECT_CALL(*params, getProperty("initialVelocity"))
        .WillOnce(Return(std::any(5)));
    
    Order order = createTestOrder("ship_001", "StartMove", params);
    
    // Регистрируем mock объект в IoC
    auto mockShip = std::make_shared<MockUObject>();
    // ... регистрация в IoC
    
    OrderInterpreter interpreter(IocContainer::getInstance());
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

TEST_F(OrderInterpreterTest, InterpretInvalidOrder_ThrowsException)
{
    Order order;
    order.objectId = "";  // Пустой id
    order.action = "StartMove";
    
    OrderInterpreter interpreter(IocContainer::getInstance());
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

TEST_F(OrderInterpreterTest, InterpretUnknownObject_ThrowsException)
{
    Order order = createTestOrder("unknown_object", "StartMove");
    
    OrderInterpreter interpreter(IocContainer::getInstance());
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

TEST_F(OrderInterpreterTest, InterpretUnknownAction_ThrowsException)
{
    Order order = createTestOrder("ship_001", "UnknownAction");
    
    // Регистрируем объект, но не регистрируем команду
    OrderInterpreter interpreter(IocContainer::getInstance());
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}
```

### Тесты защиты

```cpp
// tests/SecureOrderInterpreterTest.cpp

#include <gtest/gtest.h>
#include "SecureOrderInterpreter.hpp"
#include "Order.hpp"
#include "UObject.hpp"

class SecureOrderInterpreterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем скоупы для двух игроков
        createPlayerScope("Alice");
        createPlayerScope("Bob");
        
        // Регистрируем объекты в скоупах
        registerObjectInPlayerScope("Alice", "ship_001");
        registerObjectInPlayerScope("Alice", "ship_002");
        registerObjectInPlayerScope("Bob", "ship_003");
        registerObjectInPlayerScope("Bob", "ship_004");
    }
    
    void createPlayerScope(const std::string& playerId);
    void registerObjectInPlayerScope(
        const std::string& playerId, 
        const std::string& objectId);
    
    // Хелпер для создания Order
    Order createOrder(
        const std::string& objectId,
        const std::string& action,
        std::initializer_list<std::pair<std::string, std::any>> params = {})
    {
        Order order;
        order.objectId = objectId;
        order.action = action;
        
        auto uobj = std::make_shared<UObject>();
        for (const auto& [key, value] : params) {
            uobj->setProperty(key, value);
        }
        order.parameters = uobj;
        
        return order;
    }
};

TEST_F(SecureOrderInterpreterTest, AliceCanControlOwnShip)
{
    Order order = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    
    SecureOrderInterpreter interpreter(
        IocContainer::getInstance(), 
        "Alice"
    );
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

TEST_F(SecureOrderInterpreterTest, AliceCannotControlBobsShip)
{
    Order order = createOrder("ship_003", "StartMove", {{"initialVelocity", 5}});
    
    SecureOrderInterpreter interpreter(
        IocContainer::getInstance(), 
        "Alice"
    );
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

TEST_F(SecureOrderInterpreterTest, BobCanControlOwnShip)
{
    Order order = createOrder("ship_003", "Fire", {{"targetId", std::string("enemy_001")}});
    
    SecureOrderInterpreter interpreter(
        IocContainer::getInstance(), 
        "Bob"
    );
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

TEST_F(SecureOrderInterpreterTest, BobCannotControlAlicesShip)
{
    Order order = createOrder("ship_001", "StopMove");
    
    SecureOrderInterpreter interpreter(
        IocContainer::getInstance(), 
        "Bob"
    );
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

TEST_F(SecureOrderInterpreterTest, PlayerCanControlMultipleOwnShips)
{
    SecureOrderInterpreter interpreter(
        IocContainer::getInstance(), 
        "Alice"
    );
    
    Order order1 = createOrder("ship_001", "StartMove", {{"initialVelocity", 3}});
    Order order2 = createOrder("ship_002", "Rotate", {{"angle", 45}});
    
    EXPECT_NO_THROW(interpreter.interpret(order1));
    EXPECT_NO_THROW(interpreter.interpret(order2));
}
```

---

## 📊 План выполнения

### Этап 1: Базовая структура (1 день)
- [ ] Создать `IUObject` в common и обновить `IGameObject`
- [ ] Создать структуру директорий hw-13
- [ ] Настроить CMakeLists.txt
- [ ] Создать `Order.hpp`, `IOrderInterpreter.hpp`, `IOrderParser.hpp`

### Этап 2: Парсер и базовый интерпретатор (1-2 дня)
- [ ] Реализовать `JsonOrderParser` (адаптер JSON → Order)
- [ ] Реализовать `OrderInterpreter`
- [ ] Написать тесты для парсера и интерпретатора

### Этап 3: Команды (1 день)
- [ ] Реализовать команды: StartMove, StopMove, Fire
- [ ] Убедиться, что новые команды добавляются без изменения интерпретатора
- [ ] Добавить Rotate, SetVelocity для демонстрации расширяемости

### Этап 4: Защита через скоупы (1-2 дня)
- [ ] Реализовать `SecureOrderInterpreter`
- [ ] Настроить изоляцию скоупов для игроков
- [ ] Написать тесты защиты

### Этап 5: Финализация (1 день)
- [ ] Убедиться, что все тесты проходят
- [ ] Проверить code coverage
- [ ] Подготовить отчет
- [ ] Создать MR/PR

---

## ✅ Чек-лист для сдачи

- [ ] Код компилируется без ошибок
- [ ] Все unit-тесты проходят
- [ ] CI pipeline успешно выполняется
- [ ] `IUObject` создан в common, `IGameObject` наследуется от него
- [ ] `JsonOrderParser` парсит JSON в `Order` (адаптер)
- [ ] Интерпретатор работает с `Order`, не зависит от JSON
- [ ] Интерпретатор обрабатывает StartMove, StopMove, Fire
- [ ] Добавление новых команд не требует изменения кода интерпретатора
- [ ] Защита от чужих приказов через скоупы работает
- [ ] Тесты покрывают основные сценарии
- [ ] MR/PR создан и оформлен
- [ ] Отчет подготовлен

---

## 📚 Дополнительные материалы

- [GoF - Паттерн Interpreter](https://refactoring.guru/ru/design-patterns/interpreter)
- [nlohmann/json Documentation](https://github.com/nlohmann/json)
- [IoC Container (hw-05)](../hw-05-ioc/README.md)
- [Message Exchange (hw-08)](../hw-08-message-exchange/README.md)

---

*Документ подготовлен в рамках курса "Архитектура и паттерны проектирования" OTUS*