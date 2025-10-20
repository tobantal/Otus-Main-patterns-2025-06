cmake_minimum_required(VERSION 3.14)
project(hw_08_message_exchange VERSION 1.0.0 LANGUAGES CXX)

# Подключение nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Собираем статическую библиотеку hw08_lib
file(GLOB_RECURSE HW08_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE HW08_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(hw08_lib STATIC ${HW08_SOURCES} ${HW08_HEADERS})

# Для сборки hw08_lib необходим common/include
target_include_directories(hw08_lib
  PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Линкуем зависимости, их include-пути прокинутся автоматически
target_link_libraries(hw08_lib
  PUBLIC
    commonlib
    ioc_lib
    multithreadinglib
    nlohmann_json::nlohmann_json
)

# Серверное приложение
add_executable(server server/main.cpp)
target_link_libraries(server PRIVATE hw08_lib)

# Интеграционные тесты
enable_testing()
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_executable(server_tests ${TEST_SOURCES})
target_link_libraries(server_tests PRIVATE hw08_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(server_tests)
