project(hw_10_auth VERSION 1.0.0 LANGUAGES CXX)

# JSON
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# ========== JWT Library ==========
file(GLOB_RECURSE JWT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/include/*.hpp)
file(GLOB_RECURSE JWT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/src/*.cpp)
add_library(jwt_lib STATIC ${JWT_SOURCES} ${JWT_HEADERS})
target_include_directories(jwt_lib
  PRIVATE ${CMAKE_SOURCE_DIR}/common/include
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jwt-lib/include>
)
target_link_libraries(jwt_lib PUBLIC commonlib nlohmann_json::nlohmann_json httplib ssl crypto)

# ========== Auth Service ==========
#file(GLOB_RECURSE AUTH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/auth-service/include/*.hpp)
#file(GLOB_RECURSE AUTH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/auth-service/src/*.cpp)
#add_library(auth_service_lib STATIC ${AUTH_SOURCES} ${AUTH_HEADERS})
#target_include_directories(auth_service_lib
#  PRIVATE ${CMAKE_SOURCE_DIR}/common/include
#  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/auth-service/include>
#)
#target_link_libraries(auth_service_lib PUBLIC commonlib ioc_lib jwt_lib nlohmann_json::nlohmann_json httplib)

#add_executable(auth_server auth-service/server/main.cpp)
#target_link_libraries(auth_server PRIVATE auth_service_lib)

# ========== Battle Service ==========
#file(GLOB_RECURSE BATTLE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/battle-service/include/*.hpp)
#file(GLOB_RECURSE BATTLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/battle-service/src/*.cpp)
#add_library(battle_service_lib STATIC ${BATTLE_SOURCES} ${BATTLE_HEADERS})
#target_include_directories(battle_service_lib
#  PRIVATE ${CMAKE_SOURCE_DIR}/common/include
#  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/battle-service/include>
#)
#target_link_libraries(battle_service_lib PUBLIC commonlib ioc_lib jwt_lib hw08_lib nlohmann_json::nlohmann_json httplib)

#add_executable(battle_server battle-service/server/main.cpp)
#target_link_libraries(battle_server PRIVATE battle_service_lib)

# ========== Tests ==========
enable_testing()
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_executable(hw10_tests ${TEST_SOURCES})
target_include_directories(hw10_tests PRIVATE ${CMAKE_SOURCE_DIR}/common/include)
#target_link_libraries(hw10_tests PRIVATE jwt_lib auth_service_lib battle_service_lib hw08_lib GTest::gtest_main)
target_link_libraries(hw10_tests PRIVATE jwt_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(hw10_tests)
