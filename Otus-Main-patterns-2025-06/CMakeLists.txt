# CMakeList.txt: проект CMake для Otus-Main-patterns-2025-06; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
project(ShapeDemo)

# Подключаем include
include_directories(include)

# Основное приложение
add_executable(ShapeDemo
    src/main.cpp
    src/Rectangle.cpp
    src/Circle.cpp
 "include/MathSettings.h")

# Добавляем тесты
add_subdirectory(tests)