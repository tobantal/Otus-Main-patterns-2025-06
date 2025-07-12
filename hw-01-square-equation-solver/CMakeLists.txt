project(SquareEquationSolver)

# Подключаем include
include_directories(include)

# Основное приложение
add_executable(SquareEquationSolver
    src/main.cpp
    
    
 "include/MathSettings.h" "include/SquareEquationSolver.h" "src/SquareEquationSolver.cpp")

# Добавляем тесты
add_subdirectory(tests)