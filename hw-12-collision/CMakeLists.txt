cmake_minimum_required(VERSION 3.14)
project(hw-12-collision VERSION 1.0.0 LANGUAGES CXX)

# Сбор исходных файлов
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Создание библиотеки
add_library(collision_lib STATIC ${SOURCES} ${HEADERS})

target_include_directories(collision_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линковка с зависимостями
target_link_libraries(collision_lib
    PUBLIC
        commonlib
        exception_handler_lib
)

# Тесты
enable_testing()
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

add_executable(collision_tests ${TEST_SOURCES})

target_link_libraries(collision_tests
    PRIVATE
        collision_lib
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(collision_tests)