# Домашнее задание 13: Интерпретатор приказов

**Автор:** Anton Tobolkin  
**Курс:** Архитектура и паттерны проектирования

---

## 📋 Описание

Реализация системы обработки приказов для игровых объектов с использованием паттерна **Interpreter**. Система позволяет игрокам управлять своими космическими кораблями через JSON-приказы, при этом обеспечивая защиту от управления чужими объектами через механизм скоупов IoC-контейнера.

### Формат приказа

```json
{
  "id": "ship_001",
  "action": "StartMove",
  "initialVelocity": 5
}
```

---

## 🎯 Критерии оценки

| Критерий | Баллы | Статус |
|----------|-------|--------|
| Задача сдана на проверку | 1 | ✅ |
| Оформлен MR/PR | 1 | ✅ |
| CI | 1 | ✅ (настроен в корневом проекте) |
| Интерпретатор приказов (3 команды + расширяемость) | 3 | ✅ |
| Тесты интерпретатора | 1 | ✅ |
| Защита через скоупы | 2 | ✅ |
| Тесты защиты | 1 | ✅ |
| **ИТОГО** | **10** | ✅ |

---

## 🏗️ Архитектура

### Ключевые решения

1. **Доменная модель без JSON** — структура `Order` не зависит от формата сериализации
2. **ICommandFactory** — фабрики команд для интеграции с IoC (решает проблему типизации `std::function<std::shared_ptr<void>()>`)
3. **Защита через скоупы** — каждый игрок работает в изолированном скоупе IoC

### Структура файлов

```
hw-13-interpreter/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Order.hpp                      # Доменная модель приказа
│   ├── IOrderInterpreter.hpp          # Интерфейс интерпретатора
│   ├── IOrderParser.hpp               # Интерфейс парсера
│   ├── ICommandFactory.hpp            # Интерфейс фабрики команд
│   ├── OrderInterpreter.hpp           # Базовая реализация
│   ├── SecureOrderInterpreter.hpp     # Защищённая реализация
│   ├── JsonOrderParser.hpp            # Адаптер JSON → Order
│   └── commands/
│       ├── StartMoveCommand.hpp       
│       ├── StartMoveCommandFactory.hpp
│       ├── StopMoveCommand.hpp        
│       ├── StopMoveCommandFactory.hpp 
│       ├── FireCommand.hpp            
│       └── FireCommandFactory.hpp     
├── src/
│   ├── OrderInterpreter.cpp
│   ├── SecureOrderInterpreter.cpp
│   ├── JsonOrderParser.cpp
│   └── commands/
│       ├── StartMoveCommand.cpp
│       ├── StopMoveCommand.cpp
│       └── FireCommand.cpp
└── tests/
    ├── OrderInterpreterTest.cpp       # 10 тестов
    ├── SecureOrderInterpreterTest.cpp # 12 тестов
    ├── JsonOrderParserTest.cpp        # 9 тестов
    └── CommandsTest.cpp               # 15 тестов
```

---

## 📐 Диаграммы

### Диаграмма классов

```
┌─────────────────────────────────────────────────────────────────┐
│                            Order                                │
│  <<struct>>                                                     │
├─────────────────────────────────────────────────────────────────┤
│  + objectId: string                                             │
│  + action: string                                               │
│  + parameters: shared_ptr<IUObject>                             │
│  + isValid(): bool                                              │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      IOrderInterpreter                          │
│  <<interface>>                                                  │
├─────────────────────────────────────────────────────────────────┤
│  + interpret(order: Order): void                                │
└─────────────────────────────────────────────────────────────────┘
                              △
                              │
              ┌───────────────┴───────────────┐
              │                               │
┌─────────────────────────┐   ┌───────────────────────────────────┐
│   OrderInterpreter      │   │    SecureOrderInterpreter         │
├─────────────────────────┤   ├───────────────────────────────────┤
│  + interpret(order)     │   │  - playerId_: string              │
└─────────────────────────┘   ├───────────────────────────────────┤
                              │  + interpret(order): void         │
                              │  + getPlayerId(): string          │
                              └───────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      ICommandFactory                            │
│  <<interface>>                                                  │
├─────────────────────────────────────────────────────────────────┤
│  + create(object, params): shared_ptr<ICommand>                 │
└─────────────────────────────────────────────────────────────────┘
                              △
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
┌───────────────┐   ┌─────────────────┐   ┌─────────────────┐
│StartMoveCmd   │   │ StopMoveCmd     │   │  FireCmd        │
│   Factory     │   │   Factory       │   │   Factory       │
└───────────────┘   └─────────────────┘   └─────────────────┘
```

### Диаграмма последовательности

```
┌────────┐   ┌────────────────┐   ┌────────────────────┐   ┌──────────────┐   ┌──────────────┐
│ Client │   │ JsonOrderParser│   │ SecureOrderInterpr │   │     IoC      │   │ CmdFactory   │
└───┬────┘   └───────┬────────┘   └─────────┬──────────┘   └──────┬───────┘   └──────┬───────┘
    │                │                      │                     │                  │
    │ parse(json)    │                      │                     │                  │
    │───────────────>│                      │                     │                  │
    │                │                      │                     │                  │
    │     Order      │                      │                     │                  │
    │<───────────────│                      │                     │                  │
    │                │                      │                     │                  │
    │ interpret(order)                      │                     │                  │
    │──────────────────────────────────────>│                     │                  │
    │                │                      │                     │                  │
    │                │                      │ Scopes.Current      │                  │
    │                │                      │────────────────────>│                  │
    │                │                      │                     │                  │
    │                │                      │ Objects.ship_001    │                  │
    │                │                      │────────────────────>│                  │
    │                │                      │                     │                  │
    │                │                      │ Commands.StartMove  │                  │
    │                │                      │────────────────────>│                  │
    │                │                      │                     │                  │
    │                │                      │  ICommandFactory    │                  │
    │                │                      │<────────────────────│                  │
    │                │                      │                     │                  │
    │                │                      │ factory->create(obj, params)           │
    │                │                      │────────────────────────────────────────>
    │                │                      │                     │                  │
    │                │                      │                     │   ICommand       │
    │                │                      │<────────────────────────────────────────
    │                │                      │                     │                  │
    │                │                      │ command->execute()  │                  │
    │     OK         │                      │                     │                  │
    │<──────────────────────────────────────│                     │                  │
```

### Диаграмма скоупов (защита)

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              GLOBAL SCOPE                                   │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │  Commands (доступны из всех скоупов):                                │   │
│  │    - Commands.StartMove → StartMoveCommandFactory                    │   │
│  │    - Commands.StopMove  → StopMoveCommandFactory                     │   │
│  │    - Commands.Fire      → FireCommandFactory                         │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
│  ┌──────────────────────────────────┐  ┌──────────────────────────────────┐ │
│  │       Player.Alice Scope         │  │        Player.Bob Scope          │ │
│  │  ┌────────────────────────────┐  │  │  ┌────────────────────────────┐  │ │
│  │  │  Objects.ship_001 → Ship1  │  │  │  │  Objects.ship_003 → Ship3  │  │ │
│  │  │  Objects.ship_002 → Ship2  │  │  │  │  Objects.ship_004 → Ship4  │  │ │
│  │  └────────────────────────────┘  │  │  └────────────────────────────┘  │ │
│  └──────────────────────────────────┘  └──────────────────────────────────┘ │
│                                                                             │
│  ✓ Alice: ship_001, ship_002        ✗ Alice: ship_003, ship_004             │
│  ✗ Bob:   ship_001, ship_002        ✓ Bob:   ship_003, ship_004             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔧 Реализация

### Доменная модель Order

```cpp
struct Order {
    std::string objectId;                  // ID объекта-получателя
    std::string action;                    // Действие (StartMove, StopMove, Fire)
    std::shared_ptr<IUObject> parameters;  // Параметры без привязки к JSON
    
    bool isValid() const {
        return !objectId.empty() && !action.empty();
    }
};
```

### ICommandFactory (ключевое решение)

IoC-контейнер использует `FactoryFunction = std::function<std::shared_ptr<void>()>` без аргументов. Для передачи параметров в команды используется паттерн **Abstract Factory**:

```cpp
class ICommandFactory {
public:
    virtual ~ICommandFactory() = default;
    virtual std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) = 0;
};

// Пример фабрики
class StartMoveCommandFactory : public ICommandFactory {
public:
    std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) override 
    {
        return std::make_shared<StartMoveCommand>(object, params);
    }
};
```

### OrderInterpreter

```cpp
void OrderInterpreter::interpret(const Order& order)
{
    if (!order.isValid()) {
        throw std::runtime_error("Invalid order");
    }
    
    // 1. Разрешаем объект через IoC
    auto object = IoC::resolve<IUObject>("Objects." + order.objectId);
    
    // 2. Получаем фабрику команд
    auto factory = IoC::resolve<ICommandFactory>("Commands." + order.action);
    
    // 3. Создаём команду через фабрику
    auto command = factory->create(object, order.parameters);
    
    // 4. Выполняем
    command->execute();
}
```

### SecureOrderInterpreter

```cpp
void SecureOrderInterpreter::interpret(const Order& order)
{
    // Переключаемся в скоуп игрока
    auto scopeId = std::make_shared<std::string>("Player." + playerId_);
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    auto setCmd = IoC::resolve<ICommand>("Scopes.Current", args);
    setCmd->execute();
    
    // Теперь resolve найдёт только объекты этого игрока
    // Если объект не найден в скоупе — исключение (защита!)
    OrderInterpreter::interpret(order);
}
```

---

## 🧪 Тестирование

### Статистика тестов

| Test Suite | Тестов | Описание |
|------------|--------|----------|
| StartMoveCommandTest | 6 | Команда начала движения |
| StopMoveCommandTest | 4 | Команда остановки |
| FireCommandTest | 5 | Команда выстрела |
| JsonOrderParserTest | 9 | Парсинг JSON → Order |
| OrderInterpreterTest | 10 | Базовый интерпретатор |
| SecureOrderInterpreterTest | 12 | Защита через скоупы |
| **ИТОГО** | **46** | |

### Изоляция тестов

Тесты используют уникальные скоупы для предотвращения взаимного влияния:

```cpp
class SecureOrderInterpreterTest : public ::testing::Test {
protected:
    static int testCounter_;
    std::string testPrefix_;  // "SecTest0_", "SecTest1_", ...
    
    void SetUp() override {
        testPrefix_ = "SecTest" + std::to_string(testCounter_++) + "_";
        
        // Уникальные скоупы для каждого теста
        createPlayerScope("Alice");  // → "Player.SecTest0_Alice"
        createPlayerScope("Bob");    // → "Player.SecTest0_Bob"
        // ...
    }
};
```

### Ключевые тест-кейсы

```cpp
// Alice может управлять своим кораблём
TEST_F(SecureOrderInterpreterTest, AliceCanControlOwnShip001)
{
    Order order = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    auto interpreter = createInterpreter("Alice");
    EXPECT_NO_THROW(interpreter.interpret(order));
}

// Alice НЕ может управлять кораблём Bob'а
TEST_F(SecureOrderInterpreterTest, AliceCannotControlBobsShip003)
{
    Order order = createOrder("ship_003", "StartMove", {{"initialVelocity", 5}});
    auto interpreter = createInterpreter("Alice");
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}
```

---

## 📦 Зависимости

- **hw-05-ioc** — IoC-контейнер со скоупами
- **common** — ICommand, IUObject
- **nlohmann/json** — парсинг JSON
- **Google Test** — тестирование

---

## 🚀 Сборка и запуск

```bash
# Из корня проекта
mkdir build && cd build
cmake ..
make hw-13-interpreter-tests

# Запуск тестов
./hw-13-interpreter/hw-13-interpreter-tests
```

---

## ✅ Выполненные требования

- [x] Интерпретатор обрабатывает StartMove, StopMove, Fire
- [x] Добавление новых команд без изменения интерпретатора (через IoC + ICommandFactory)
- [x] Защита от чужих приказов через скоупы IoC
- [x] Полное покрытие тестами (46 тестов)
- [x] Доменная модель Order не зависит от JSON
- [x] Чистая архитектура: парсер → интерпретатор → IoC → команда

