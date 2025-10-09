project(hw-06-adapter VERSION 1.0.0 LANGUAGES CXX)

# Подключаем подпроекты
# Предполагается, что корневой CMakeLists.txt уже сделал add_subdirectory(hw-05-ioc)
# и теперь мы находимся в hw-06-adapter, а hw-05-ioc собран как таргет ioc_lib.

# 1) Генератор адаптеров
add_subdirectory(adapter-generator)

# Пути для генерации
set(INTERFACES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/adapters)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/adapters)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(INTERFACE_FILES
    ${INTERFACES_DIR}/IMovable.hpp
    ${INTERFACES_DIR}/IRotatable.hpp
)

set(GENERATED_FILES
    ${GENERATED_DIR}/*.hpp
)

add_custom_command(
    OUTPUT ${GENERATED_FILES}
    COMMAND $<TARGET_FILE:adapter_generator>
        ${INTERFACES_DIR}
        ${GENERATED_DIR}
    DEPENDS
        adapter_generator
        ${INTERFACE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating adapters from interfaces..."
    VERBATIM
)

add_custom_target(generate_adapters ALL
    DEPENDS ${GENERATED_FILES}
)

# 2) Сборка библиотеки adapter_ioc_lib
include_directories(${CMAKE_CURRENT_SOURCE_DIR})  # чтобы видеть include/
file(GLOB_RECURSE LIB_SOURCES_ADAPTER "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(adapter_ioc_lib ${LIB_SOURCES_ADAPTER})

target_include_directories(adapter_ioc_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include          # ICommand.hpp, IoC.hpp и пр.
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces # CommandGetter.hpp, CommandSetter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src              # если есть .hpp в src
        ${CMAKE_CURRENT_BINARY_DIR}/generated/adapters
)

target_link_libraries(adapter_ioc_lib
    PUBLIC ioc_lib
)

add_dependencies(adapter_ioc_lib generate_adapters)

# 3) Тесты
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES_ADAPTER "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(run_adapter_tests ${TEST_SOURCES_ADAPTER})

target_include_directories(run_adapter_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/include/adapters
        ${CMAKE_CURRENT_SOURCE_DIR}/include/custom_implementations
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/generated/adapters
)

target_link_libraries(run_adapter_tests 
    PRIVATE
        adapter_ioc_lib
        GTest::gtest_main
)

add_dependencies(run_adapter_tests generate_adapters)
include(GoogleTest)
gtest_discover_tests(run_adapter_tests)