cmake_minimum_required(VERSION 3.14)
project(hw-08-message-exchange VERSION 1.0.0 LANGUAGES CXX)

add_executable(server main.cpp)

# Для Linux может понадобиться pthread
if(UNIX)
    target_link_libraries(server pthread)
endif()
