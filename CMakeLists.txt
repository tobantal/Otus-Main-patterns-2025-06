cmake_minimum_required (VERSION 3.14)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Otus-Main-patterns-2025-06")

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# сборка общей библиотеки для всех проектов (включает ICommand.hpp)
#add_subdirectory(common)

# сборка библиотеки Http-сервера на основе cpp-httplib
#add_subdirectory(httplib-wrapper)

# Включите подпроекты.
#add_subdirectory ("hw-01-square-equation-solver")
#add_subdirectory ("hw-02-abstractions")
#add_subdirectory("hw-03-solid-and-exceptions")

#add_subdirectory ("hw-04-macro-commands")

# Подключаем hw-05-ioc: собираем её как библиотеку
#add_subdirectory(hw-05-ioc)

# Подключаем hw-06-adapter
# зависит от hw-05-ioc (раскомментировать при необходимости)
#add_subdirectory(hw-06-adapter)

# Подключаем hw-07-multithreading: собираем её как библиотеку
#add_subdirectory(hw-07-multithreading)

# Использует библиотеки ioc, multithreading
#add_subdirectory(hw-08-message-exchange)

# Сборка проекта с авторизацией
#add_subdirectory(hw-10-auth)

# Сборка проекта со сменой состояний
#add_subdirectory(hw-11-state)


# Сборка финального проекта
add_subdirectory(hw-14-final-project)