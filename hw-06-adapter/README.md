# Генератор адаптеров на C++17

## Структура проекта

```bash
adapter-generator/
├── include/
│   ├── interfaces/
│   ├── parsers/
│   ├── generators/
│   ├── models/
│   └── utils/
├── src/
│   ├── parsers/
│   ├── generators/
│   ├── models/
│   └── utils/
├── examples/
├── tests/
├── main.cpp
└── CMakeLists.txt
```

## Система аннотаций для кастомных реализаций

Для указания кастомной реализации метода используется специальная аннотация в комментарии:

```cpp
/**
 * @custom_impl ClassName::methodName
 */
virtual ReturnType methodName() const = 0;
```

Где `ClassName::methodName` - это ссылка на статический метод класса, который содержит кастомную реализацию.

Например:

```cpp
    /**
     * @brief Получить вектор скорости объекта
     * @custom_impl MovableImplementations::getVelocity
     * @return Вектор скорости с учетом направления и модуля скорости
     */
    virtual Vector2D getVelocity() const = 0;
```
## Использование генератора

### Компиляция

```bash
mkdir build && cd build
cmake ..
make
```

### Запуск

```bash
# Генерация отдельных файлов для каждого адаптера
./adapter_generator ./examples/interfaces ./output

# Генерация одного файла со всеми адаптерами  
./adapter_generator --single-file ./examples/interfaces ./output

# Справка
./adapter_generator --help
```

### Интеграция в проект

```cpp
# В CMakeLists.txt
find_program(ADAPTER_GENERATOR adapter_generator)

if(ADAPTER_GENERATOR)
    generate_adapters(YourTargetName
        INPUT_DIR ${CMAKE_SOURCE_DIR}/include/interfaces
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated
    )
else()
    message(WARNING "Adapter generator not found. Install it first.")
endif()
```
----
Этот генератор адаптеров поддерживает:

1. **Парсинг C++ интерфейсов** с помощью регулярных выражений
2. **Автоматическую генерацию адаптеров** для паттерна Adapter
3. **Кастомные реализации методов** через аннотации `@custom_impl`
4. **Разделение на отдельные файлы** согласно принципу "один класс - один файл"
5. **Полную документацию Doxygen** для всех компонентов
6. **Интеграцию с CMake** для автоматизации сборки
7. **Гибкую настройку** через аргументы командной строки

----

## Критерии успешной реализации

### Функциональные требования (10 баллов)
- [ ] **Задание сдано на проверку** (1 балл)
- [ ] **Реализован генератор адаптеров** (3 балла)
  - Поиск абстрактных классов в заголовочных файлах
  - Анализ методов интерфейсов
  - Генерация корректного кода адаптеров
- [ ] **Реализованы тесты на генератор** (2 балла)
  - Unit-тесты парсинга интерфейсов
  - Тесты генерации кода по шаблонам
  - Интеграционные тесты с IoC контейнером
- [ ] **Определена стратегия для IoC** (2 балла)
  - Автоматическая регистрация адаптеров
  - Фабричные методы создания адаптеров
- [ ] **Обработан случай void методов** (2 балла)
  - Поддержка методов без возвращаемого значения
  - Правильная генерация команд для выполнения