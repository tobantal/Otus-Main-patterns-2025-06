# Паттерн Интерпретатор (Interpreter)

## Лекция по паттерну проектирования

**Автор:** Anton Tobolkin  
**Курс:** Архитектура и паттерны проектирования  
**Тема:** Занятие 13 - Паттерн Interpreter

---

## 1. Введение

### 1.1 Определение

**Интерпретатор** — поведенческий шаблон проектирования, который для заданного языка определяет представление его грамматики, а также интерпретатор предложений этого языка.

Также известен как **Little (Small) Language** — "маленький язык".

### 1.2 Назначение

Паттерн применяется когда:
- Некоторая задача возникает часто
- Проявления задачи можно представить в виде предложений на простом языке
- Требуется гибкость настройки без изменения кода

### 1.3 Мотивация

Если определённая задача встречается достаточно часто, имеет смысл:
1. Представить её конкретные проявления в виде предложений на простом языке
2. Создать интерпретатор, который решает задачу, анализируя предложения этого языка

---

## 2. Структура паттерна

### 2.1 Диаграмма классов

```
                    ┌─────────────┐
                    │   Context   │
                    └─────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
        ▼                  ▼                  ▼
┌─────────────┐    ┌───────────────────┐    
│   Client    │───▶│AbstractExpression │◀────────────┐
└─────────────┘    │  + interpret()    │             │
                   └───────────────────┘             │
                           △                         │
           ┌───────────────┴───────────────┐        │
           │                               │        │
┌─────────────────────┐   ┌──────────────────────────┐
│TerminalExpression   │   │NonterminalExpression     │
│  + interpret()      │   │  + interpret()           │◇───┘
└─────────────────────┘   └──────────────────────────┘
```

### 2.2 Участники

#### AbstractExpression (Абстрактное выражение)
- Объявляет абстрактную операцию `interpret()`, общую для всех узлов AST
- В C++ реализуется как абстрактный класс с чисто виртуальным методом

```cpp
/**
 * @brief Абстрактное выражение - базовый интерфейс для всех узлов AST
 */
class IExpression {
public:
    virtual ~IExpression() = default;
    
    /**
     * @brief Интерпретирует выражение в заданном контексте
     * @param context Контекст выполнения
     * @return Результат интерпретации
     */
    virtual std::any interpret(Context& context) = 0;
};
```

#### TerminalExpression (Терминальное выражение)
- Реализует операцию `interpret()` для терминальных символов грамматики
- Терминалы — это "листья" дерева: числа, переменные, константы
- Необходим отдельный экземпляр для каждого терминального символа

```cpp
/**
 * @brief Терминальное выражение - число
 */
class NumberExpression : public IExpression {
public:
    explicit NumberExpression(double value) : value_(value) {}
    
    std::any interpret(Context& context) override {
        return value_;
    }
    
private:
    double value_;
};

/**
 * @brief Терминальное выражение - переменная
 */
class VariableExpression : public IExpression {
public:
    explicit VariableExpression(const std::string& name) : name_(name) {}
    
    std::any interpret(Context& context) override {
        return context.getVariable(name_);
    }
    
private:
    std::string name_;
};
```

#### NonterminalExpression (Нетерминальное выражение)
- По одному классу требуется для каждого грамматического правила
- Хранит переменные экземпляра типа `AbstractExpression` (дочерние узлы)
- Рекурсивно вызывает `interpret()` для дочерних узлов

```cpp
/**
 * @brief Нетерминальное выражение - бинарная операция
 */
class BinaryExpression : public IExpression {
public:
    BinaryExpression(
        std::shared_ptr<IExpression> left,
        std::shared_ptr<IExpression> right,
        OperatorType op)
        : left_(left), right_(right), op_(op) {}
    
    std::any interpret(Context& context) override {
        auto leftVal = std::any_cast<double>(left_->interpret(context));
        auto rightVal = std::any_cast<double>(right_->interpret(context));
        
        switch (op_) {
            case OperatorType::Add: return leftVal + rightVal;
            case OperatorType::Sub: return leftVal - rightVal;
            case OperatorType::Mul: return leftVal * rightVal;
            case OperatorType::Div: return leftVal / rightVal;
            default: throw std::runtime_error("Unknown operator");
        }
    }
    
private:
    std::shared_ptr<IExpression> left_;
    std::shared_ptr<IExpression> right_;
    OperatorType op_;
};
```

#### Context (Контекст)
- Содержит информацию, глобальную по отношению к интерпретатору
- Хранит значения переменных, состояние выполнения

```cpp
/**
 * @brief Контекст выполнения интерпретатора
 */
class Context {
public:
    void setVariable(const std::string& name, double value) {
        variables_[name] = value;
    }
    
    double getVariable(const std::string& name) const {
        auto it = variables_.find(name);
        if (it == variables_.end()) {
            throw std::runtime_error("Unknown variable: " + name);
        }
        return it->second;
    }
    
private:
    std::unordered_map<std::string, double> variables_;
};
```

#### Client (Клиент)
- Строит (или получает в готовом виде) абстрактное синтаксическое дерево (AST)
- Вызывает операцию `interpret()`

---

## 3. Абстрактное синтаксическое дерево (AST)

### 3.1 Определение

**AST (Abstract Syntax Tree)** — конечное помеченное ориентированное дерево, в котором:
- Внутренние вершины сопоставлены с операторами языка
- Листья — с соответствующими операндами (переменные и константы)

### 3.2 Пример: выражение `(A - B) * C`

```
        ┌───┐
        │ * │ ← NonterminalExpression (умножение)
        └─┬─┘
      ┌───┴───┐
      │       │
    ┌─┴─┐   ┌─┴─┐
    │( )│   │ C │ ← TerminalExpression (переменная)
    └─┬─┘   └───┘
      │
    ┌─┴─┐
    │ - │ ← NonterminalExpression (вычитание)
    └─┬─┘
  ┌───┴───┐
┌─┴─┐   ┌─┴─┐
│ A │   │ B │ ← TerminalExpression (переменные)
└───┘   └───┘
```

### 3.3 Построение AST в C++

```cpp
// Выражение: (A - B) * C
auto A = std::make_shared<VariableExpression>("A");
auto B = std::make_shared<VariableExpression>("B");
auto C = std::make_shared<VariableExpression>("C");

auto subtraction = std::make_shared<BinaryExpression>(
    A, B, OperatorType::Sub);
    
auto multiplication = std::make_shared<BinaryExpression>(
    subtraction, C, OperatorType::Mul);

// Выполнение
Context ctx;
ctx.setVariable("A", 10);
ctx.setVariable("B", 3);
ctx.setVariable("C", 2);

auto result = std::any_cast<double>(multiplication->interpret(ctx));
// result = (10 - 3) * 2 = 14
```

---

## 4. Грамматика

### 4.1 Формы Бэкуса-Наура (БНФ)

Грамматика — способ описания структурированного текста.

**Пример грамматики арифметических выражений:**

```
ФОРМУЛА ::= ПЕРЕМЕННАЯ 
          | ЧИСЛО 
          | ( ФОРМУЛА ) 
          | ФОРМУЛА + ФОРМУЛА 
          | ФОРМУЛА - ФОРМУЛА 
          | ФОРМУЛА * ФОРМУЛА 
          | ФОРМУЛА / ФОРМУЛА

ЧИСЛО   ::= ЦИФРА | ЧИСЛО ЦИФРА
ЦИФРА   ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

### 4.2 Терминалы и нетерминалы

| Тип | Описание | Примеры |
|-----|----------|---------|
| **Терминал** | Выражение, встречающееся в языке напрямую | число, переменная, `+`, `-`, `if` |
| **Нетерминал** | Искусственное выражение, состоящее из других выражений | ФОРМУЛА, ЧИСЛО, ОПЕРАТОР |

---

## 5. DSL (Domain-Specific Language)

### 5.1 Определение

**DSL** (Domain-Specific Language) — предметно-ориентированный язык, специализированный для конкретной области применения.

### 5.2 Примеры DSL

| DSL | Область применения |
|-----|-------------------|
| SQL | Работа с базами данных |
| Regex | Обработка текста |
| CSS | Стилизация веб-страниц |
| LaTeX | Верстка документов |
| CMake | Сборка проектов |
| nginx config | Конфигурация веб-сервера |

### 5.3 Способы создания DSL

#### 5.3.1 Внешний DSL (External DSL)
- Придумать свой язык
- Описать его грамматику
- Написать парсер

**Преимущества:**
- Полная свобода — любые операторы и конструкции
- Синтаксис максимально приближен к предметной области

**Недостатки:**
- Сложно реализовать
- Требуется грамматика и генерация парсера

#### 5.3.2 Внутренний DSL (Internal DSL / Embedded DSL)
- Использовать возможности имеющегося языка программирования
- Максимально походить на DSL, но писать обычные классы и функции

**Преимущества:**
- Относительно просто реализовать
- Поддержка IDE и компилятора

**Недостатки:**
- Ограничены возможностями хост-языка

### 5.4 Пример DSL для игровых команд

```
// DSL для приказов космическим кораблям
ПРИКАЗ ::= { "id": ID, "action": ACTION, ПАРАМЕТРЫ }

ACTION ::= "StartMove" | "StopMove" | "Fire" | "Rotate"

ПАРАМЕТРЫ ::= "initialVelocity": ЧИСЛО
            | "angle": ЧИСЛО
            | "targetId": ID
```

**Примеры приказов:**
```json
{
  "id": "548",
  "action": "StartMove",
  "initialVelocity": 2
}

{
  "id": "549",
  "action": "Fire",
  "targetId": "enemy_001"
}

{
  "id": "550",
  "action": "Rotate",
  "angle": 45
}
```

---

## 6. Компиляторы и интерпретаторы

### 6.1 Трансляция программы

**Трансляция** — преобразование программы на одном языке в программу на другом языке.

### 6.2 Компилятор

Транслирует весь исходный код в машинный код **до** выполнения:

```
Исходный код → [Компилятор] → Машинный код → [Выполнение]
```

### 6.3 Интерпретатор

#### Простой интерпретатор
Последовательно, команда за командой, обрабатывает исходный код:

```
Исходный код → [Интерпретатор] → Результат
                    ↺ (построчно)
```

#### Интерпретатор компилирующего типа
Прежде чем выполнить код, конвертирует его в промежуточный формат:

```
Исходный код → [Предварительная обработка] → Промежуточный код → [Интерпретатор] → Результат
```

Примеры: Python (.pyc файлы), Java (.class файлы)

---

## 7. Реализация интерпретатора приказов

### 7.1 Структура приказа

```cpp
/**
 * @brief Приказ для игрового объекта
 */
struct Order {
    std::string objectId;      // ID объекта-получателя
    std::string action;        // Действие для выполнения
    nlohmann::json parameters; // Параметры действия
};
```

### 7.2 Интерфейс интерпретатора

```cpp
/**
 * @brief Интерфейс интерпретатора приказов
 */
class IOrderInterpreter {
public:
    virtual ~IOrderInterpreter() = default;
    
    /**
     * @brief Интерпретирует и выполняет приказ
     * @param order JSON с приказом
     * @throws std::runtime_error при ошибке интерпретации
     */
    virtual void interpret(const nlohmann::json& order) = 0;
};
```

### 7.3 Реализация через IoC

```cpp
/**
 * @brief Интерпретатор приказов, использующий IoC для создания команд
 */
class OrderInterpreter : public IOrderInterpreter {
public:
    void interpret(const nlohmann::json& order) override {
        // Извлекаем данные из приказа
        std::string objectId = order["id"];
        std::string action = order["action"];
        
        // Получаем объект из IoC
        auto uobject = IoC::resolve<IUObject>("Objects." + objectId);
        
        // Создаем команду через IoC
        // Ключ формируется как "Commands.<action>"
        auto command = IoC::resolve<ICommand>(
            "Commands." + action,
            {uobject, order}
        );
        
        // Выполняем команду
        command->execute();
    }
};
```

### 7.4 Регистрация команд в IoC

```cpp
// Регистрация фабрик команд
IoC::resolve<ICommand>("IoC.Register", {
    std::make_shared<std::string>("Commands.StartMove"),
    std::make_shared<std::function<std::shared_ptr<void>()>>(
        [](auto obj, auto params) {
            return std::make_shared<StartMoveCommand>(obj, params);
        }
    )
})->execute();

IoC::resolve<ICommand>("IoC.Register", {
    std::make_shared<std::string>("Commands.StopMove"),
    std::make_shared<std::function<std::shared_ptr<void>()>>(
        [](auto obj, auto params) {
            return std::make_shared<StopMoveCommand>(obj, params);
        }
    )
})->execute();

IoC::resolve<ICommand>("IoC.Register", {
    std::make_shared<std::string>("Commands.Fire"),
    std::make_shared<std::function<std::shared_ptr<void>()>>(
        [](auto obj, auto params) {
            return std::make_shared<FireCommand>(obj, params);
        }
    )
})->execute();
```

---

## 8. Защита от чужих приказов

### 8.1 Проблема

Игрок A не должен иметь возможность отдавать приказы объектам игрока B.

### 8.2 Решение через скоупы IoC

Каждый игрок работает в своём изолированном скоупе:

```cpp
/**
 * @brief Установка скоупа для игрока
 */
void setPlayerScope(const std::string& playerId) {
    auto scopeId = std::make_shared<std::string>("Player." + playerId);
    std::vector<std::shared_ptr<void>> args = {scopeId};
    
    // Создаем скоуп если не существует
    auto createScopeCommand = IoC::resolve<ICommand>("Scopes.New", args);
    try {
        createScopeCommand->execute();
    } catch (...) {
        // Скоуп уже существует
    }
    
    // Устанавливаем как текущий
    auto setCurrentCommand = IoC::resolve<ICommand>("Scopes.Current", args);
    setCurrentCommand->execute();
}
```

### 8.3 Регистрация объектов в скоупе игрока

```cpp
/**
 * @brief Регистрация игрового объекта в скоупе игрока
 */
void registerObjectForPlayer(
    const std::string& playerId,
    const std::string& objectId,
    std::shared_ptr<IUObject> object)
{
    // Переключаемся в скоуп игрока
    setPlayerScope(playerId);
    
    // Регистрируем объект
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        [object]() { return std::static_pointer_cast<void>(object); }
    );
    
    auto key = std::make_shared<std::string>("Objects." + objectId);
    std::vector<std::shared_ptr<void>> args = {key, factory};
    
    auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
    registerCommand->execute();
}
```

### 8.4 Проверка доступа

```cpp
/**
 * @brief Безопасный интерпретатор с проверкой доступа
 */
class SecureOrderInterpreter : public IOrderInterpreter {
public:
    explicit SecureOrderInterpreter(const std::string& playerId)
        : playerId_(playerId) {}
    
    void interpret(const nlohmann::json& order) override {
        // Устанавливаем скоуп текущего игрока
        setPlayerScope(playerId_);
        
        std::string objectId = order["id"];
        
        // Попытка разрешить объект в скоупе игрока
        // Если объект не принадлежит игроку — исключение
        try {
            auto uobject = IoC::resolve<IUObject>("Objects." + objectId);
            
            std::string action = order["action"];
            auto command = IoC::resolve<ICommand>(
                "Commands." + action,
                {uobject, order}
            );
            
            command->execute();
        } catch (const std::exception& e) {
            throw std::runtime_error(
                "Access denied: object " + objectId + 
                " does not belong to player " + playerId_
            );
        }
    }
    
private:
    std::string playerId_;
};
```

---

## 9. Расширяемость интерпретатора

### 9.1 Добавление новых команд без изменения кода

Максимальная оценка (3 балла) за интерпретатор требует, чтобы преподаватель не смог привести пример приказа, для которого нужно изменить код.

**Решение:** Все команды регистрируются в IoC динамически.

```cpp
/**
 * @brief Универсальный интерпретатор
 * 
 * Для добавления новой команды достаточно зарегистрировать её в IoC
 * без изменения кода интерпретатора
 */
class UniversalOrderInterpreter : public IOrderInterpreter {
public:
    void interpret(const nlohmann::json& order) override {
        std::string objectId = order["id"];
        std::string action = order["action"];
        
        // Динамическое получение объекта
        auto uobject = IoC::resolve<IUObject>("Objects." + objectId);
        
        // Динамическое создание команды
        // Новые команды добавляются через IoC.Register
        auto command = IoC::resolve<ICommand>(
            "Commands." + action,
            {uobject, order}
        );
        
        command->execute();
    }
};
```

### 9.2 Обработка не только приказов игровым объектам

Для получения 3 баллов интерпретатор должен обрабатывать любые приказы:

```cpp
/**
 * @brief Максимально расширяемый интерпретатор
 */
class ExtensibleInterpreter : public IOrderInterpreter {
public:
    void interpret(const nlohmann::json& order) override {
        std::string action = order["action"];
        
        // Проверяем, есть ли в приказе id объекта
        if (order.contains("id")) {
            // Приказ игровому объекту
            interpretObjectOrder(order);
        } else {
            // Системный приказ (например, создание игры, настройки)
            interpretSystemOrder(order);
        }
    }
    
private:
    void interpretObjectOrder(const nlohmann::json& order) {
        std::string objectId = order["id"];
        std::string action = order["action"];
        
        auto uobject = IoC::resolve<IUObject>("Objects." + objectId);
        auto command = IoC::resolve<ICommand>(
            "Commands." + action,
            {uobject, order}
        );
        
        command->execute();
    }
    
    void interpretSystemOrder(const nlohmann::json& order) {
        std::string action = order["action"];
        
        auto command = IoC::resolve<ICommand>(
            "SystemCommands." + action,
            {order}
        );
        
        command->execute();
    }
};
```

---

## 10. Паттерн Visitor как альтернатива

### 10.1 Проблема с методом Interpret()

Что если мы хотим:
- Вычислять выражение
- Преобразовать в строку
- Преобразовать в картинку
- Передать на сервер

Метод `interpret()` тогда должен возвращать разные типы результатов.

### 10.2 Решение: паттерн Visitor

```cpp
/**
 * @brief Visitor для обхода AST
 */
class IExpressionVisitor {
public:
    virtual ~IExpressionVisitor() = default;
    virtual void visitNumber(NumberExpression* expr) = 0;
    virtual void visitVariable(VariableExpression* expr) = 0;
    virtual void visitBinary(BinaryExpression* expr) = 0;
};

/**
 * @brief Выражение с поддержкой Visitor
 */
class IExpression {
public:
    virtual ~IExpression() = default;
    virtual void accept(IExpressionVisitor& visitor) = 0;
};

/**
 * @brief Visitor для вычисления значения
 */
class EvaluateVisitor : public IExpressionVisitor {
public:
    double getResult() const { return result_; }
    
    void visitNumber(NumberExpression* expr) override {
        result_ = expr->getValue();
    }
    
    void visitVariable(VariableExpression* expr) override {
        result_ = context_.getVariable(expr->getName());
    }
    
    void visitBinary(BinaryExpression* expr) override {
        expr->getLeft()->accept(*this);
        double left = result_;
        
        expr->getRight()->accept(*this);
        double right = result_;
        
        switch (expr->getOperator()) {
            case OperatorType::Add: result_ = left + right; break;
            case OperatorType::Sub: result_ = left - right; break;
            // ...
        }
    }
    
private:
    double result_ = 0;
    Context& context_;
};

/**
 * @brief Visitor для преобразования в строку
 */
class ToStringVisitor : public IExpressionVisitor {
public:
    std::string getResult() const { return result_; }
    
    void visitNumber(NumberExpression* expr) override {
        result_ = std::to_string(expr->getValue());
    }
    
    void visitVariable(VariableExpression* expr) override {
        result_ = expr->getName();
    }
    
    void visitBinary(BinaryExpression* expr) override {
        expr->getLeft()->accept(*this);
        std::string left = result_;
        
        expr->getRight()->accept(*this);
        std::string right = result_;
        
        result_ = "(" + left + " " + 
                  operatorToString(expr->getOperator()) + 
                  " " + right + ")";
    }
    
private:
    std::string result_;
};
```

---

## 11. Заключение

### 11.1 Когда применять паттерн Interpreter

✅ **Используйте когда:**
- Есть язык для интерпретации с простой грамматикой
- Эффективность не критична
- Требуется гибкость настройки без перекомпиляции

❌ **Не используйте когда:**
- Грамматика сложная (лучше использовать генераторы парсеров)
- Требуется высокая производительность
- Выражения простые и не требуют расширения

### 11.2 Преимущества

- Легко изменять и расширять грамматику
- Реализация грамматики проста
- Добавление новых способов интерпретации (через Visitor)

### 11.3 Недостатки

- Сложные грамматики трудно поддерживать
- Для каждого правила грамматики нужен класс

### 11.4 Связанные паттерны

| Паттерн | Связь |
|---------|-------|
| **Composite** | AST — это реализация Composite |
| **Visitor** | Альтернатива множественным методам interpret() |
| **Flyweight** | Для разделения терминальных символов |
| **Iterator** | Для обхода дерева |

---

## 12. Литература

1. **"Паттерны объектно-ориентированного проектирования"** — Гамма, Хелм, Джонсон, Влиссидес (GoF)
2. **"Head First. Паттерны проектирования"** — Фримен, Робсон
3. **"Language Oriented Programming"** — Martin Ward, 1994
4. **Boost Spirit** — https://www.boost.org/doc/libs/release/libs/spirit/

---

*Документ подготовлен в рамках курса "Архитектура и паттерны проектирования" OTUS*
