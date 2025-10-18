project(hw-05-ioc VERSION 1.0.1 LANGUAGES CXX)

# Собираем исходные файлы
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создание библиотеки IoC
add_library(ioc_lib ${SOURCES})

# Делаем include/ публичным для подпроектов
target_include_directories(ioc_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<TARGET_PROPERTY:commonlib,INTERFACE_INCLUDE_DIRECTORIES>  # заголовки commonlib
)

# Настройка GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Тесты для IoC
enable_testing()
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(ioc_tests ${TEST_SOURCES})
target_link_libraries(ioc_tests
  PRIVATE
    commonlib
    ioc_lib
    GTest::gtest_main
    GTest::gmock_main
)
include(GoogleTest)
gtest_discover_tests(ioc_tests)

# Демонстрационное приложение (в том числе для интеграции)
add_executable(ioc_demo "${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp")
target_link_libraries(ioc_demo
  PRIVATE ioc_lib
)
