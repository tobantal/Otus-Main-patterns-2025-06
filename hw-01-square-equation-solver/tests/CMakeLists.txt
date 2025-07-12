# Подключаем исходники
set(TEST_SOURCES
    main_test.cpp
    SquareEquationSolverTests.cpp
)

add_executable(SquareEquationSolverTests ${TEST_SOURCES}
    ../src/SquareEquationSolver.cpp
)

target_include_directories(SquareEquationSolverTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(SquareEquationSolverTests gtest_main)

include(GoogleTest)
gtest_discover_tests(SquareEquationSolverTests)