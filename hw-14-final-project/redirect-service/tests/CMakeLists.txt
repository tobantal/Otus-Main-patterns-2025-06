# Tests for redirect-service
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Enable testing
enable_testing()

# Fetch GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
)

# Для Windows: предотвращаем перезапись компиляторных флагов
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(redirect-service-test
    RuleParserTest.cpp
    DSLEvaluatorTest.cpp
    RedirectServiceTest.cpp
    RulesCacheTest.cpp
    RedirectHandlerTest.cpp
)

target_link_libraries(redirect-service-test
    redirect-service-lib
    gtest
    gtest_main
    gmock
    gmock_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(redirect-service-test)

message(STATUS "Redirect Service tests configured")