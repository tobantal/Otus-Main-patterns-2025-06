# Space Battle - Macro Commands

## Описание

Домашняя работа №4: Реализация макрокоманд для игры "Космическая битва".

Проект демонстрирует применение паттернов проектирования Command, Composite и Adapter для создания гибкой системы управления игровыми объектами с поддержкой:
- Движения с расходом топлива
- Поворота с изменением вектора скорости
- Композиции команд через макрокоманды
- Расширяемой архитектуры без модификации существующего кода

## Архитектура

### Ключевые компоненты:

#### 1. Система топлива
- **IFuelable** - интерфейс для объектов с топливом
- **FuelableAdapter** - адаптер IGameObject → IFuelable
- **CheckFuelCommand** - проверка наличия топлива
- **BurnFuelCommand** - сжигание топлива

#### 2. Макрокоманды
- **MacroCommand** - композитная команда для последовательного выполнения
- **MoveWithFuelCommand** - движение с проверкой и расходом топлива
- **RotateWithVelocityChangeCommand** - поворот с изменением вектора скорости

#### 3. Изменение скорости
- **IVelocityChangeable** - интерфейс для объектов с изменяемой скоростью
- **ChangeVelocityCommand** - команда изменения вектора скорости при повороте

### Применение SOLID принципов:

1. **Single Responsibility**: Каждый класс отвечает за одну функцию
2. **Open/Closed**: Новые возможности добавляются без изменения существующего кода
3. **Liskov Substitution**: MacroCommand может использоваться везде, где ICommand
4. **Interface Segregation**: Интерфейсы разделены по функциональности
5. **Dependency Inversion**: Зависимости только от абстракций

## Требования

- C++17 или выше
- CMake 3.20+
- GoogleTest для тестирования
- Visual Studio 2022 (Windows) или GCC/Clang (Linux/macOS)

## Структура проекта

```
hw-04-macro-commands/
├── include/              # Заголовочные файлы (.hpp)
│   ├── IFuelable.hpp
│   ├── FuelableAdapter.hpp
│   ├── CheckFuelCommand.hpp
│   ├── BurnFuelCommand.hpp
│   ├── MacroCommand.hpp
│   ├── ChangeVelocityCommand.hpp
│   └── ...
├── src/                  # Реализация (.cpp)
│   ├── FuelableAdapter.cpp
│   ├── CheckFuelCommand.cpp
│   ├── BurnFuelCommand.cpp
│   ├── MacroCommand.cpp
│   └── ...
├── tests/                # Unit-тесты
│   ├── CheckFuelCommandTest.cpp
│   ├── BurnFuelCommandTest.cpp
│   ├── MacroCommandTest.cpp
│   └── ...
├── examples/             # Примеры использования
│   └── main.cpp
├── CMakeLists.txt        # Конфигурация сборки
└── README.md             # Документация
```

## Использование

### Простое движение с топливом:
```cpp
auto spaceShip = std::make_shared<SpaceShip>();
spaceShip->setProperty("Position", Vector2D(0, 0));
spaceShip->setProperty("Velocity", 10);
spaceShip->setProperty("FuelLevel", 100);
spaceShip->setProperty("FuelConsumption", 10);

auto moveCommand = MoveWithFuelCommand::create(spaceShip);
moveCommand->execute();  // Проверит топливо, переместит, сожжет топливо
```

### Поворот с изменением скорости:
```cpp
auto rotateCommand = RotateWithVelocityChangeCommand::create(spaceShip, true);
rotateCommand->execute();  // Повернет и изменит вектор скорости
```

### Создание сложной последовательности:
```cpp
std::vector<std::shared_ptr<ICommand>> commands = {
    std::make_shared<CheckFuelCommand>(fuelable),
    std::make_shared<RotateCommand>(rotatable),
    std::make_shared<ChangeVelocityCommand>(velocityChangeable),
    std::make_shared<BurnFuelCommand>(fuelable)
};

MacroCommand complexManeuver(commands);
complexManeuver.execute();  // Выполнит все команды последовательно
```

## Тестирование

Проект включает полный набор unit-тестов для всех компонентов:

- **CheckFuelCommandTest** - тесты проверки топлива
- **BurnFuelCommandTest** - тесты сжигания топлива
- **MacroCommandTest** - тесты макрокоманд
- **ChangeVelocityCommandTest** - тесты изменения скорости
- **MoveWithFuelCommandTest** - тесты движения с топливом
- **RotateWithVelocityChangeCommandTest** - тесты поворота со скоростью

### Покрытие тестами:
- Успешные сценарии выполнения
- Обработка исключений (недостаток топлива)
- Граничные случаи
- Валидация входных данных

## CI/CD

Проект настроен для автоматической сборки через GitHub Actions:
- Ubuntu (GCC) и Windows (MSVC)
- Debug и Release конфигурации
- Автоматический запуск всех тестов

## Оценка

Выполненные пункты домашнего задания:

- [x] Домашнее задание сдано - 1 балл
- [x] Реализована команда CheckFuelCommand - 1 балл
- [x] Написаны тесты к CheckFuelCommand - 1 балл
- [x] Реализована команда BurnFuelCommand - 1 балл
- [x] Написаны тесты к BurnFuelCommand - 1 балл
- [x] Реализована макрокоманда движения и тесты - 1 балл
- [x] Написаны тесты к MacroCommand - 1 балл
- [x] Реализована команда ChangeVelocityCommand - 1 балл
- [x] Написаны тесты к ChangeVelocityCommand - 1 балл
- [x] Реализована команда поворота с изменением скорости - 1 балл

**Итого выполнено пунктов: 10/10**

## Автор

Антон Тоболкин