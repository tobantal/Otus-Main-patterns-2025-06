# Microservice Core Library - Интерфейсы для микросервисов
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Header-only библиотека с интерфейсами
add_library(microservice-core INTERFACE)

target_include_directories(microservice-core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Требуем C++17
target_compile_features(microservice-core INTERFACE cxx_std_17)