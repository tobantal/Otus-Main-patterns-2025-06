cmake_minimum_required(VERSION 3.20)
project(SpaceBattle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем папки
include_directories(include)

# Собираем библиотеку с основной логикой
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(SpaceBattleLib STATIC ${SOURCES})
target_include_directories(SpaceBattleLib PUBLIC include)

# Собираем тесты
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(SpaceBattleTests ${TEST_SOURCES})
target_link_libraries(SpaceBattleTests SpaceBattleLib GTest::gtest GTest::gtest_main)

# Регистрируем тесты
include(GoogleTest)
gtest_discover_tests(SpaceBattleTests)

# Создаем исполняемый файл (опционально)
add_executable(SpaceBattle main.cpp)
target_link_libraries(SpaceBattle SpaceBattleLib)