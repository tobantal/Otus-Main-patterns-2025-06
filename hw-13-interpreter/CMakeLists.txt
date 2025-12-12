project(hw-13-interpreter VERSION 1.0.0 LANGUAGES CXX)

# Собираем исходные файлы
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создание библиотеки интерпретатора
add_library(interpreter_lib ${SOURCES})

# Подключаем nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(json)

# Делаем include/ публичным для подпроектов
target_include_directories(interpreter_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<TARGET_PROPERTY:commonlib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:ioc_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

# Линкуем зависимости
target_link_libraries(interpreter_lib
  PUBLIC
    commonlib
    ioc_lib
    nlohmann_json::nlohmann_json
)

# Настройка GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Тесты для интерпретатора
enable_testing()
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(interpreter_tests ${TEST_SOURCES})
target_link_libraries(interpreter_tests
  PRIVATE
    interpreter_lib
    GTest::gtest_main
    GTest::gmock_main
)
include(GoogleTest)
gtest_discover_tests(interpreter_tests)