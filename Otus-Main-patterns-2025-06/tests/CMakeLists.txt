# Подключаем исходники
set(TEST_SOURCES
    main_test.cpp
    RectangleTests.cpp
    CircleTests.cpp
)

add_executable(ShapeTests ${TEST_SOURCES}
    ../src/Rectangle.cpp
    ../src/Circle.cpp
)

target_include_directories(ShapeTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ShapeTests gtest_main)

include(GoogleTest)
gtest_discover_tests(ShapeTests)