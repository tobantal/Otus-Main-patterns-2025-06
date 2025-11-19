# Tests for microservice-core
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
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(redirect-service-test
    RuleParserTest.cpp
    DSLEvaluatorTest.cpp
)

target_link_libraries(redirect-service-test
    redirect-service-lib
    gtest
    gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(redirect-service-test)