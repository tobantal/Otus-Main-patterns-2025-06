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
)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Добавляем тесты
add_subdirectory(tests)