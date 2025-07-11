#include <gtest/gtest.h>
#include "Rectangle.h"

TEST(RectangleTest, AreaAndPerimeter) {
    Rectangle r(3.0, 4.0);
    EXPECT_DOUBLE_EQ(r.area(), 12.0);
    EXPECT_DOUBLE_EQ(r.perimeter(), 14.0);
}