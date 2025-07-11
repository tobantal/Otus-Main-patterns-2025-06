#include <gtest/gtest.h>
#include "Circle.h"
#include <corecrt_math_defines.h>

TEST(CircleTest, AreaAndPerimeter) {
    Circle c(2.0);
    EXPECT_NEAR(c.area(), M_PI * 4.0, 1e-5);
    EXPECT_NEAR(c.perimeter(), 2 * M_PI * 2.0, 1e-5);
}