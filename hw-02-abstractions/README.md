# Space Battle - Movement System

Система движения объектов для игры "Космическая битва", реализованная с применением принципов SOLID и паттернов проектирования.

## Описание

Проект реализует систему движения игровых объектов в двумерном пространстве. Поддерживает:
- Прямолинейное равномерное движение
- Поворот объектов вокруг собственной оси
- Расширяемую архитектуру для новых типов объектов

## Архитектура

### Основные принципы:
- **SOLID принципы**: Каждый класс имеет единственную ответственность
- **Command Pattern**: Операции инкапсулированы в команды
- **Adapter Pattern**: Универсальные объекты адаптируются к специализированным интерфейсам
- **Interface Segregation**: Интерфейсы разделены по функциональности

### Структура проекта:
```bash
├── include/          # Заголовочные файлы
├── src/              # Реализация
├── tests/            # Unit-тесты
├── CMakeLists.txt    # Конфигурация сборки
└── README.md         # Документация
```
## Требования

- C++17 или выше
- CMake 3.20+
- GoogleTest для тестирования

## Сборка и запуск

### Windows (Visual Studio):
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Linux/macOS:
```bash
mkdir build
cd build
cmake ..
make
```

### Запуск тестов:
```bash
cd build
ctest --output-on-failure
```

### Использование
```cpp
#include "SpaceShip.hpp"
#include "MoveCommand.hpp"
#include "MovableAdapter.hpp"

// Создание космического корабля
auto spaceShip = std::make_shared<SpaceShip>();
spaceShip->setProperty("Position", Vector2D(10, 5));
spaceShip->setProperty("Velocity", 8);
spaceShip->setProperty("DirectionsNumber", 360);
spaceShip->setProperty("Direction", 45);
spaceShip->setProperty("AngularVelocity", 30);

// Создание адаптера и команды движения
auto movableAdapter = std::make_shared<MovableAdapter>(spaceShip);
MoveCommand moveCommand(movableAdapter);

// Выполнение движения
moveCommand.execute();
```

### Тестирование
Проект включает полный набор unit-тестов, покрывающих:

- Успешные сценарии использования
- Обработку ошибок
- Граничные случаи
- Соответствие требованиям задания

### CI/CD
Настроена автоматическая сборка и тестирование через GitHub Actions для:

- Ubuntu и Windows
- Debug и Release конфигурации
- Автоматический запуск тестов