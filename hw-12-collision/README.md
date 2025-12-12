# ДЗ-12: Проверка коллизий с паттерном Chain of Responsibility

**Автор:** Anton Tobolkin

## Цель работы

Научиться применять паттерн "Цепочка обязанностей" для эффективной проверки коллизий игровых объектов с использованием пространственного разбиения на регионы.

## Выполненные требования

| Критерий | Баллы | Статус |
|----------|-------|--------|
| Сдача на проверку | 1 | ✅ |
| MR/PR оформление | 1 | ✅ |
| CI | 1 | ✅ |
| Правильное формирование списков объектов в окрестностях | 1 | ✅ |
| Полная реализация команды UpdateRegionCommand | 2 | ✅ |
| Минимум один тест | 1 | ✅ |
| Полное покрытие тестами | 2 | ✅ |
| Произвольное количество систем окрестностей | 2 | ✅ |
| Тесты для нескольких систем окрестностей | 1 | ✅ |

## Архитектура решения

### Диаграмма классов

```
┌─────────────────────┐     ┌──────────────────────┐
│   IRegionResolver   │     │    IRegionManager    │
├─────────────────────┤     ├──────────────────────┤
│ +getRegion()        │     │ +addObject()         │
│ +getCellSize()      │     │ +removeObject()      │
└─────────┬───────────┘     │ +moveObject()        │
          │                 │ +getObjectsInRegion()│
          ▼                 └───────────┬──────────┘
┌─────────────────────┐                │
│ GridRegionResolver  │                ▼
├─────────────────────┤     ┌─────────────────────┐
│ -cellSize_          │     │   RegionManager     │
│ -offset_            │◄────┤ -resolver_          │
└─────────────────────┘     │ -regions_           │
                            └─────────────────────┘
                                      │
                                      ▼
┌─────────────────────────────────────────────────┐
│            UpdateRegionCommand                  │
├─────────────────────────────────────────────────┤
│ -object_: shared_ptr<IGameObject>               │
│ -regionManager_: shared_ptr<IRegionManager>     │
│ -collisionChecker_:shared_ptr<ICollisionChecker>│
│ -collisionMacro_: shared_ptr<MacroCommand>&     │
├─────────────────────────────────────────────────┤
│ +execute()                                      │
│ -getStoredRegion()                              │
│ -storeRegion()                                  │
└─────────────────────────────────────────────────┘
                      │
                      ▼
┌────────────────────────────────────────────────────┐
│         MultiRegionCollisionSystem                 │
├────────────────────────────────────────────────────┤
│ -managers_: vector<shared_ptr<RegionManager>>      │
│ -collisionMacros_: vector<shared_ptr<MacroCommand>>│
│ -cellSize_, numSystems_                            │
├────────────────────────────────────────────────────┤
│ +addObject(), removeObject(), updateObject()       │
│ +getCollisionCommands()                            │
└────────────────────────────────────────────────────┘
```

### Решение проблемы границ

Объекты около границы региона могут столкнуться с объектами соседнего региона. Решение — несколько систем окрестностей со смещением:

```
Система 0 (смещение 0):        Система 1 (смещение 50):
┌─────┬─────┐                  ┌─────┬─────┐
│  A  │  B  │  ← разные        │ A B │     │  ← один регион!
├─────┼─────┤    регионы       ├─────┼─────┤
│     │     │                  │     │     │
└─────┴─────┘                  └─────┴─────┘
      ↑                              ↑
   граница                    граница сдвинута
```

Формула смещения i-й системы из N: `offset = (i / N) * cellSize`

## Реализация

### UpdateRegionCommand (п.1 задания)

Команда выполняет:
1. Определение текущей окрестности объекта по позиции
2. Сравнение с сохранённой окрестностью
3. Перемещение объекта между окрестностями при изменении
4. Создание команд проверки коллизий со всеми соседями
5. Обновление макрокоманды

```cpp
void UpdateRegionCommand::execute() {
    RegionId newRegion = regionManager_->getResolver().getRegion(*object_);
    
    if (hasStoredRegion()) {
        RegionId oldRegion = getStoredRegion();
        if (newRegion != oldRegion) {
            regionManager_->moveObject(object_, oldRegion, newRegion);
            storeRegion(newRegion);
        }
    } else {
        regionManager_->addObject(object_);
        storeRegion(newRegion);
    }
    
    auto neighbors = regionManager_->getObjectsInRegion(newRegion);
    std::vector<std::shared_ptr<ICommand>> collisionCommands;
    
    for (const auto& neighbor : neighbors) {
        if (neighbor.get() != object_.get()) {
            collisionCommands.push_back(
                std::make_shared<CheckCollisionCommand>(object_, neighbor, collisionChecker_)
            );
        }
    }
    
    collisionMacro_ = !collisionCommands.empty() 
        ? std::make_shared<MacroCommand>(collisionCommands) 
        : nullptr;
}
```

### MultiRegionCollisionSystem (п.3 задания)

Поддерживает произвольное количество систем окрестностей:

```cpp
MultiRegionCollisionSystem::MultiRegionCollisionSystem(
    double cellSize, std::shared_ptr<ICollisionChecker> checker, int numSystems)
{
    for (int i = 0; i < numSystems; ++i) {
        double offsetFraction = static_cast<double>(i) / numSystems;
        Vector2D offset(cellSize * offsetFraction, cellSize * offsetFraction);
        
        auto resolver = std::make_shared<GridRegionResolver>(cellSize, offset);
        managers_.push_back(std::make_shared<RegionManager>(resolver));
    }
}
```

## Тестовое покрытие

### GridRegionResolverTest (11 тестов)
- Определение региона по позиции
- Работа с отрицательными координатами
- Работа со смещением сетки
- Валидация параметров

### RegionManagerTest (12 тестов)
- Добавление/удаление объектов
- Перемещение между регионами
- Работа с пустыми регионами
- Валидация параметров

### UpdateRegionCommandTest (10 тестов)
- Первое добавление объекта
- Повторные вызовы без перемещения
- Перемещение при смене региона
- Создание команд проверки коллизий
- Валидация параметров

### CheckCollisionCommandTest (6 тестов)
- Вызов checker при execute()
- Получение объектов
- Валидация параметров

### MultiRegionCollisionSystemTest (14 тестов)
- Создание с разным числом подсистем (1, 2, 4)
- Добавление/удаление объектов во все подсистемы
- **Ключевой тест:** объекты на границе попадают в один регион во второй системе
- Генерация команд коллизий
- Проверка смещений в многосистемной конфигурации

## Структура проекта

```
hw-12-collision/
├── CMakeLists.txt
├── include/
│   ├── RegionId.hpp
│   ├── IRegionResolver.hpp
│   ├── GridRegionResolver.hpp
│   ├── IRegionManager.hpp
│   ├── RegionManager.hpp
│   ├── ICollisionChecker.hpp
│   ├── CheckCollisionCommand.hpp
│   ├── UpdateRegionCommand.hpp
│   └── MultiRegionCollisionSystem.hpp
├── src/
│   ├── GridRegionResolver.cpp
│   ├── RegionManager.cpp
│   ├── CheckCollisionCommand.cpp
│   ├── UpdateRegionCommand.cpp
│   └── MultiRegionCollisionSystem.cpp
└── tests/
    ├── GridRegionResolverTest.cpp
    ├── RegionManagerTest.cpp
    ├── CheckCollisionCommandTest.cpp
    ├── UpdateRegionCommandTest.cpp
    └── MultiRegionCollisionSystemTest.cpp
```

## Зависимости

- **commonlib** — ICommand, IGameObject, CommandException
- **exception_handler_lib** — MacroCommand, Vector2D, SpaceShip
- **Google Test / Google Mock** — тестирование

## Сборка и запуск тестов

```bash
# Из корня проекта
mkdir build && cd build
cmake ..
make collision_tests
./hw-12-collision/collision_tests
```

## Выводы

В ходе выполнения работы реализована система проверки коллизий с использованием пространственного разбиения. Паттерн Chain of Responsibility применён через создание цепочки команд проверки коллизий, объединённых в макрокоманду.

Ключевые решения:
1. Разделение на интерфейсы (IRegionResolver, IRegionManager, ICollisionChecker) обеспечивает гибкость и тестируемость
2. Хранение текущего региона в свойствах объекта позволяет отслеживать перемещения
3. Система с N смещёнными окрестностями решает проблему границ регионов
4. Полное покрытие тестами гарантирует корректность реализации