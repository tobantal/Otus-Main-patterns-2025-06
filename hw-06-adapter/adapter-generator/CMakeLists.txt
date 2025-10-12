project(AdapterGenerator VERSION 1.0.0 LANGUAGES CXX)

# Включаем заголовочные файлы
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Исходные файлы для библиотеки (без main.cpp)
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Создаем статическую библиотеку (для переиспользования в тестах)
add_library(adapter_generator_lib ${LIB_SOURCES} ${HEADERS})

# Создаем основной исполняемый файл
add_executable(adapter_generator main.cpp)
target_link_libraries(adapter_generator adapter_generator_lib)

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

# Сбор тестовых файлов
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Создание исполняемого файла для тестов
add_executable(run_tests ${TEST_SOURCES})

# Линковка с библиотеками
target_link_libraries(run_tests 
    adapter_generator_lib
    GTest::gtest_main
    GTest::gmock_main
)

# Добавление тестов
include(GoogleTest)
gtest_discover_tests(run_tests)

# Установка
install(TARGETS adapter_generator DESTINATION bin)