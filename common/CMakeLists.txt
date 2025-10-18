# common/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(commonlib VERSION 1.0.1 LANGUAGES CXX)

# Это header-only библиотека — определяем INTERFACE target
add_library(commonlib INTERFACE)

# Делаем include/ публичным для подпроектов
target_include_directories(commonlib
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
