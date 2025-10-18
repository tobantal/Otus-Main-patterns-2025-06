cmake_minimum_required(VERSION 3.14)
project(hw-08-message-exchange VERSION 1.0.0 LANGUAGES CXX)

# ============================================
# Подключение nlohmann/json через FetchContent
# ============================================
include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

# ============================================
# Основной исполняемый файл (сервер)
# ============================================
add_executable(server server/main.cpp)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(UNIX)
    target_link_libraries(server pthread)
endif()

# ============================================
# Тесты
# ============================================
add_executable(server_tests tests/server_test.cpp)

target_include_directories(server_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(server_tests 
    PRIVATE 
    gtest_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(server_tests)
