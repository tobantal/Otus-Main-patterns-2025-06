# Redirect Service - Микросервис переадресации
# Author: Anton Tobolkin

cmake_minimum_required(VERSION 3.14)

# Подавляем warning от Boost.DI
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-nonnull)
endif()

# Создаем исполняемый файл
add_executable(redirect-service
    main.cpp
    src/RedirectServiceApp.cpp
    src/handlers/GetUserHandler.cpp
    src/services/UserService.cpp
    src/repositories/InMemoryUserRepository.cpp
)

# Подключаем заголовки
target_include_directories(redirect-service PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${boostdi_SOURCE_DIR}/include
)

# Линкуем с библиотеками
target_link_libraries(redirect-service PRIVATE
    microservice-boost
    microservice-core
)

# Требуем C++17
target_compile_features(redirect-service PRIVATE cxx_std_17)