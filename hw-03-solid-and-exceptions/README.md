# Space Battle Exception Handling System

## Описание проекта

Система обработки исключений для игры "Космическая битва", реализованная с использованием паттерна Command и соответствующая принципам SOLID.

### Основные возможности:
- **Гибкая обработка исключений** - различные стратегии обработки без модификации try-catch блоков
- **Паттерн Command** - инкапсуляция операций в объекты
- **Очередь команд** - последовательная обработка команд с перехватом исключений
- **Логирование** - запись информации об ошибках в файл
- **Повторение команд** - автоматическое повторение неудачных операций
- **Комбинированные стратегии** - "повторить, затем логировать" и "повторить дважды, затем логировать"

## Структура проекта

```
hw-03-solid-and-exceptions/
├── CMakeLists.txt              # Конфигурация CMake
├── README.md                   # Этот файл
├── demo/                       # Демонстрационное приложение
│   ├── main.cpp              
├── include/                    # Заголовочные файлы
│   ├── ICommand.hpp            # Интерфейс команды
│   ├── IExceptionHandler.hpp   # Интерфейс обработчика исключений
│   ├── CommandQueue.hpp        # Очередь команд
│   ├── LogCommand.hpp          # Команда логирования
│   ├── RetryCommand.hpp        # Команда повторения
│   ├── DoubleRetryCommand.hpp  # Команда двойного повторения
│   ├── ExceptionHandlers.hpp   # Обработчики исключений
│   └── TestCommands.hpp        # Тестовые команды
├── src/                        # Реализация классов
│   ├── CommandQueue.cpp
│   ├── LogCommand.cpp
│   ├── RetryCommand.cpp
│   ├── DoubleRetryCommand.cpp
│   ├── ExceptionHandlers.cpp
│   └── TestCommands.cpp
│  
└── tests/                      # Unit-тесты
    ├── test_LogCommand.cpp
    ├── test_RetryCommand.cpp
    ├── test_ExceptionHandlers.cpp
    └── test_CommandQueue.cpp
```

## Требования

- C++17 или выше
- CMake 3.14+
- Google Test (автоматически загружается через CMake)
- Компилятор с поддержкой C++17 (GCC 7+, Clang 5+, MSVC 2017+)

## Сборка проекта

### Linux/MacOS

```bash
# Создать директорию для сборки
mkdir build
cd build

# Сгенерировать файлы сборки
cmake ..

# Собрать проект
make

# Или с указанием количества потоков
make -j4
```

### Windows (Visual Studio)

```bash
# Создать директорию для сборки
mkdir build
cd build

# Сгенерировать решение Visual Studio
cmake .. -G "Visual Studio 17 2022"

# Открыть .sln файл в Visual Studio или собрать из командной строки
cmake --build . --config Release
```

## Запуск

### Демонстрационное приложение

```bash
# Linux/MacOS
./demo

# Windows
demo.exe
```

### Unit-тесты

```bash
# Linux/MacOS
./run_tests

# Windows
run_tests.exe

# Или через CTest
ctest --verbose
```

## Использование

### Базовый пример

```cpp
#include "CommandQueue.hpp"
#include "ExceptionHandlers.hpp"
#include "TestCommands.hpp"

int main() {
    // Создаем очередь команд
    CommandQueue queue;
    
    // Создаем и устанавливаем обработчик исключений
    auto logHandler = std::make_shared<LogExceptionHandler>(&queue, "error.log");
    queue.setDefaultHandler(logHandler);
    
    // Добавляем команды
    queue.push(std::make_shared<SuccessCommand>("Task1"));
    queue.push(std::make_shared<FailCommand>("Task2"));
    
    // Обрабатываем все команды
    queue.processAll();
    
    return 0;
}
```

### Стратегия "Повторить, затем логировать"

```cpp
// Создаем обработчик для стратегии
auto handler = std::make_shared<RetryThenLogHandler>(&queue, "error.log");

// Регистрируем для конкретных типов
queue.registerHandler<FailCommand, std::runtime_error>(handler);
queue.registerHandler<RetryCommand, std::runtime_error>(handler);

// При первой ошибке команда будет повторена
// При второй ошибке - записана в лог
```

### Стратегия "Повторить дважды, затем логировать"

```cpp
// Создаем обработчик для стратегии
auto handler = std::make_shared<DoubleTryThenLogHandler>(&queue, "error.log");

// Регистрируем для всех необходимых типов
queue.registerHandler<FailCommand, std::runtime_error>(handler);
queue.registerHandler<RetryCommand, std::runtime_error>(handler);
queue.registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

// Команда будет повторена два раза, затем залогирована
```

## Принципы SOLID

### Single Responsibility Principle
- `LogCommand` - только логирование
- `RetryCommand` - только повторение
- `CommandQueue` - только управление очередью

### Open-Closed Principle
- Новые стратегии обработки добавляются без изменения существующего кода
- Новые типы команд реализуют интерфейс `ICommand`

### Liskov Substitution Principle
- Все команды взаимозаменяемы через интерфейс `ICommand`
- Все обработчики взаимозаменяемы через `IExceptionHandler`

### Interface Segregation Principle
- Минималистичные интерфейсы с одним методом
- Классы не зависят от неиспользуемых методов

### Dependency Inversion Principle
- Зависимость от абстракций (интерфейсов), а не конкретных классов
- `CommandQueue` работает с `ICommand` и `IExceptionHandler`

## Тестирование

Проект включает полное покрытие unit-тестами:

- **test_LogCommand** - тесты команды логирования
- **test_RetryCommand** - тесты команды повторения
- **test_ExceptionHandlers** - тесты всех обработчиков
- **test_CommandQueue** - тесты очереди команд

### Запуск тестов с фильтром

```bash
# Запустить только тесты LogCommand
./run_tests --gtest_filter=LogCommandTest.*

# Запустить конкретный тест
./run_tests --gtest_filter=CommandQueueTest.ProcessAll
```

## Логирование

Система автоматически создает лог-файлы с временными метками:

```
[2024-01-20 15:30:45] Command [FailCommand] failed with exception: Intentional failure
```

## Автор

Тоболкин Антон

## Лицензия

MIT