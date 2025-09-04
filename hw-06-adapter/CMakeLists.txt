# Имя проекта
set(PROJECT_NAME hw-06-adapter)

cmake_minimum_required(VERSION 3.14)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Добавление путей для поиска заголовочных файлов
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Сбор исходных файлов
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Создание библиотеки
add_library(project_lib ${SOURCES})

# Настройка GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Включение тестирования
enable_testing()

# Сбор тестовых файлов
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Создание исполняемого файла для тестов
add_executable(run_tests ${TEST_SOURCES})

# Линковка с библиотеками
target_link_libraries(run_tests 
    project_lib
    GTest::gtest_main
    GTest::gmock_main
)

# Добавление тестов
include(GoogleTest)
gtest_discover_tests(run_tests)

# Демонстрационное приложение
add_executable(demo demo/main.cpp)
target_link_libraries(demo project_lib)

