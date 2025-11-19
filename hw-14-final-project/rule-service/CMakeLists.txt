# Rule Service
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Подавляем warning от Boost.DI
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-nonnull)
endif()

# ============================================================================
# FetchContent для libpqxx
# ============================================================================
include(FetchContent)

# Сначала получаем libpq (PostgreSQL C library)
find_package(PostgreSQL REQUIRED)

# Получаем libpqxx
FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG 7.9.2  # Стабильная версия, можно обновить
)

# Опции для libpqxx (отключаем ненужное)
set(SKIP_BUILD_TEST ON CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_TEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libpqxx)

# ============================================================================
# Библиотека с бизнес-логикой (для тестов)
# ============================================================================
file(GLOB_RECURSE REDIRECT_SERVICE_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(rule-service-lib ${REDIRECT_SERVICE_LIB_SOURCES})

target_include_directories(rule-service-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${boostdi_SOURCE_DIR}/include
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(rule-service-lib PUBLIC
    microservice-boost
    microservice-core
    nlohmann_json::nlohmann_json
    pqxx
    ${PostgreSQL_LIBRARIES}
)

target_compile_features(rule-service-lib PUBLIC cxx_std_17)

# ============================================================================
# Исполняемый файл (только main.cpp)
# ============================================================================
add_executable(rule-service ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(rule-service PRIVATE
    rule-service-lib
)
