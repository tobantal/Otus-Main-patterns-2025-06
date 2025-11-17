# Redirect Service - Микросервис переадресации
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Подавляем warning от Boost.DI
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-nonnull)
endif()

# Собираем все cpp файлы рекурсивно
file(GLOB_RECURSE REDIRECT_SERVICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Создаем исполняемый файл
add_executable(redirect-service ${REDIRECT_SERVICE_SOURCES})

# Подключаем заголовки
target_include_directories(redirect-service PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${boostdi_SOURCE_DIR}/include
)

# Линкуем с библиотеками
target_link_libraries(redirect-service PRIVATE
    microservice-boost
    microservice-core
    nlohmann_json::nlohmann_json
)

# Требуем C++17
target_compile_features(redirect-service PRIVATE cxx_std_17)