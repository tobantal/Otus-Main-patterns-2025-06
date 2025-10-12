cmake_minimum_required(VERSION 3.14)
project(hw-07-multithreading VERSION 1.0.0 LANGUAGES CXX)

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
add_library(multithreading_lib ${SOURCES} ${HEADERS})

# Подключение pthread для многопоточности
find_package(Threads REQUIRED)
target_link_libraries(multithreading_lib Threads::Threads)

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
    multithreading_lib
    GTest::gtest_main
    GTest::gmock_main
    Threads::Threads
)

# Добавление тестов
include(GoogleTest)
gtest_discover_tests(run_tests)

# Вывод информации о конфигурации
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")