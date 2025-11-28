# Redirect Service - Микросервис переадресации
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Подавляем warning от Boost.DI
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-nonnull)
endif()

# ============================================================================
# Библиотека с бизнес-логикой (для тестов)
# ============================================================================
file(GLOB_RECURSE REDIRECT_SERVICE_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(redirect-service-lib ${REDIRECT_SERVICE_LIB_SOURCES})

target_include_directories(redirect-service-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${boostdi_SOURCE_DIR}/include
    ${microservice_core_SOURCE_DIR}/include
    ${microservice_boost_SOURCE_DIR}/include
)

target_link_libraries(redirect-service-lib PUBLIC
    microservice-boost
    microservice-core
    nlohmann_json::nlohmann_json
)

target_compile_features(redirect-service-lib PUBLIC cxx_std_17)

# ============================================================================
# Исполняемый файл (только main.cpp)
# ============================================================================
add_executable(redirect-service ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(redirect-service PRIVATE
    redirect-service-lib
)