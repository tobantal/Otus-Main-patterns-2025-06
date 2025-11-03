cmake_minimum_required(VERSION 3.10)
project(hw_10_auth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JSON
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# ========== JWT Library ==========
file(GLOB_RECURSE JWT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/include/*.hpp)
file(GLOB_RECURSE JWT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/src/*.cpp)
add_library(jwt_lib STATIC ${JWT_SOURCES} ${JWT_HEADERS})
target_include_directories(jwt_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/include
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/src
)
target_link_libraries(jwt_lib PUBLIC commonlib nlohmann_json::nlohmann_json httplib ssl crypto)

# ========== Auth Service ==========
file(GLOB_RECURSE AUTH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/auth-service/include/*.hpp)
file(GLOB_RECURSE AUTH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/auth-service/src/*.cpp)
add_library(auth_service_lib STATIC ${AUTH_SOURCES} ${AUTH_HEADERS})
target_include_directories(auth_service_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/include
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/auth-service/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/auth-service/src
)
target_link_libraries(auth_service_lib PUBLIC commonlib ioc_lib jwt_lib nlohmann_json::nlohmann_json httplib)

add_executable(auth_server auth-service/server/main.cpp)
target_link_libraries(auth_server PRIVATE auth_service_lib)

# ========== Battle Service ==========
file(GLOB_RECURSE BATTLE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/battle-service/include/*.hpp)
file(GLOB_RECURSE BATTLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/battle-service/src/*.cpp)
add_library(battle_service_lib STATIC ${BATTLE_SOURCES} ${BATTLE_HEADERS})
target_include_directories(battle_service_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/include
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/battle-service/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/battle-service/src
)
target_link_libraries(battle_service_lib PUBLIC commonlib ioc_lib jwt_lib hw08_lib nlohmann_json::nlohmann_json httplib)

add_executable(battle_server battle-service/server/main.cpp)
target_link_libraries(battle_server PRIVATE battle_service_lib)

# ========== Tests ==========
enable_testing()

# Копируем ключи в build директорию ДЛЯ ТЕСТОВ
add_custom_target(copy_keys ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/keys
    ${CMAKE_BINARY_DIR}/keys
    COMMENT "Copying keys directory to build folder"
)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_executable(hw10_tests ${TEST_SOURCES})
target_link_libraries(hw10_tests PRIVATE jwt_lib auth_service_lib battle_service_lib hw08_lib GTest::gtest_main)

# hw10_tests ЗАВИСИТ от copy_keys
add_dependencies(hw10_tests copy_keys)

include(GoogleTest)
gtest_discover_tests(hw10_tests)
