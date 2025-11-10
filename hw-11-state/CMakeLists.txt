project(hw_11_state VERSION 1.0.0 LANGUAGES CXX)

# Собираем библиотеку statelib из cpp-файлов
file(GLOB_RECURSE STATE_SOURCES
    src/*.cpp
)
file(GLOB_RECURSE STATE_HEADERS
    include/*.hpp
)

add_library(statelib
    ${STATE_SOURCES}
    ${STATE_HEADERS}
)

target_include_directories(statelib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Линкуем с другой нашей библиотекой commonlib
target_link_libraries(statelib
    PUBLIC
        commonlib           # ICommand, ThreadSafeMap, ThreadSafeQueue
)

# Потоки
find_package(Threads REQUIRED)
target_link_libraries(statelib
    PUBLIC
        Threads::Threads
)

# Тесты
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE STATE_TEST_SOURCES
    tests/*.cpp
)

add_executable(state_tests
    ${STATE_TEST_SOURCES}
)

target_include_directories(state_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(state_tests
    PRIVATE
        statelib
        commonlib
        GTest::gtest_main
        Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(state_tests)
