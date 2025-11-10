cmake_minimum_required(VERSION 3.14)
project(httplib-wrapper VERSION 0.26.0 LANGUAGES CXX)

# Header-only библиотека
add_library(httplib INTERFACE)

target_include_directories(httplib
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Минимальные требования для cpp-httplib
target_compile_features(httplib INTERFACE cxx_std_17)
