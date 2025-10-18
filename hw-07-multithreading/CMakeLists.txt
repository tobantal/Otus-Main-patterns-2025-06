cmake_minimum_required(VERSION 3.14)
project(hw_07_multithreading VERSION 1.0.0 LANGUAGES CXX)

# Собираем библиотеку multithreadinglib из cpp-файлов
file(GLOB_RECURSE MT_SOURCES
    src/*.cpp
)
file(GLOB_RECURSE MT_HEADERS
    include/*.hpp
)

add_library(multithreadinglib
    ${MT_SOURCES}
    ${MT_HEADERS}
)

target_include_directories(multithreadinglib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<TARGET_PROPERTY:commonlib,INTERFACE_INCLUDE_DIRECTORIES>  # заголовки commonlib
)

find_package(Threads REQUIRED)
target_link_libraries(multithreadinglib
    PUBLIC
        Threads::Threads
)

# Тесты библиотеки (интеграционные и юнит)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE MT_TEST_SOURCES
    tests/*.cpp
)

add_executable(multithreading_tests
    ${MT_TEST_SOURCES}
)

target_include_directories(multithreading_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(multithreading_tests
    PRIVATE
        multithreadinglib
        GTest::gtest_main
        Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(multithreading_tests)
