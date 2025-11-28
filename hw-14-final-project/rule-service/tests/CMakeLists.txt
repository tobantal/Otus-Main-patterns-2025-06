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
add_executable(rule-service-test
    CreateRuleHandlerTest.cpp
    DeleteRuleHandlerTest.cpp
    UpdateRuleHandlerTest.cpp
    GetRuleHandlerTest.cpp
    ListRulesHandlerTest.cpp
    InvalidateCacheHandlerTest.cpp
    CacheInvalidatorSettingsTest.cpp
    HttpCacheInvalidatorTest.cpp
    RuleServiceTest.cpp
    InMemoryRuleRepositoryTest.cpp
)

target_link_libraries(rule-service-test
    rule-service-lib
    gtest
    gtest_main
    gmock
    gmock_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(rule-service-test)