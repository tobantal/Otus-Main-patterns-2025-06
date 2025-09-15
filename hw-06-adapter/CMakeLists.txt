project(hw-06-adapter VERSION 1.0.0 LANGUAGES CXX)

# Подключаем подпроекты
# Предполагается, что корневой CMakeLists.txt уже сделал add_subdirectory(hw-05-ioc)
# и теперь мы находимся в hw-06-adapter, а hw-05-ioc собран как таргет ioc_lib.

# 1) Генератор адаптеров
add_subdirectory(adapter-generator)

# Определяем пути для генерации
set(INTERFACES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces)
# TODO: генерировать адаптеры в bin папку
set(GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/adapters)

# Создаем директорию для сгенерированных файлов, если её нет
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Список интерфейсов для обработки
set(INTERFACE_FILES
    ${INTERFACES_DIR}/IMovable.hpp
    ${INTERFACES_DIR}/IRotatable.hpp
)

# Список файлов, которые будут сгенерированы
set(GENERATED_FILES
    ${GENERATED_DIR}/MovableAdapter.hpp
    ${GENERATED_DIR}/RotatableAdapter.hpp
)

# Команда для генерации адаптеров
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

# Создаем таргет для генерации адаптеров
add_custom_target(generate_adapters ALL
    DEPENDS ${GENERATED_FILES}
)

# 2) Собираем основное демонстрационное приложение
#add_executable(adapter_demo
#    demo/main.cpp
#)

# Подключаем include-пути (если в demo нужны заголовки адаптеров)
#target_include_directories(adapter_demo
#    PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/adapter_generator/include
#)

# Линкуем с библиотеками IoC и адаптер-генератором
#target_link_libraries(adapter_demo
#    PRIVATE
#        ioc_lib
#        adapter_generator_lib
#)

# Включаем заголовочные файлы
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Исходные файлы и заголовки
file(GLOB_RECURSE LIB_SOURCES_ADAPTER "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создаем статическую библиотеку из исходников и заголовков
add_library(adapter_ioc_lib ${LIB_SOURCES_ADAPTER})
target_include_directories(adapter_ioc_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(adapter_ioc_lib PUBLIC ioc_lib)

# Настройка GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Включение тестирования
enable_testing()

# Поиск всех тестовых файлов
file(GLOB_RECURSE TEST_SOURCES_ADAPTER "tests/*.cpp")

# Создаем исполняемый файл для тестов
add_executable(run_adapter_tests ${TEST_SOURCES_ADAPTER})

# Линковка тестового приложения с библиотеками
target_link_libraries(run_adapter_tests 
    adapter_ioc_lib
    GTest::gtest_main
    GTest::gmock_main
)

# Автоматическая регистрация тестов
include(GoogleTest)
gtest_discover_tests(run_adapter_tests)
