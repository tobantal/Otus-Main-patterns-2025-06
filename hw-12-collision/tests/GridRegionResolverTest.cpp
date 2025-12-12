#include <gtest/gtest.h>
#include "GridRegionResolver.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тесты для GridRegionResolver
 */
class GridRegionResolverTest : public ::testing::Test {
protected:
    std::shared_ptr<SpaceShip> object;
    
    void SetUp() override {
        object = std::make_shared<SpaceShip>();
    }
    
    void setPosition(double x, double y) {
        object->setProperty("Position", Vector2D(x, y));
    }
};

/**
 * @test Объект в центре ячейки (0,0)
 */
TEST_F(GridRegionResolverTest, ObjectInCenterOfFirstCell) {
    GridRegionResolver resolver(100.0);
    setPosition(50, 50);
    
    RegionId region = resolver.getRegion(*object);
    
    EXPECT_EQ(0, region.x);
    EXPECT_EQ(0, region.y);
}

/**
 * @test Объект во второй ячейке по X
 */
TEST_F(GridRegionResolverTest, ObjectInSecondCellX) {
    GridRegionResolver resolver(100.0);
    setPosition(150, 50);
    
    RegionId region = resolver.getRegion(*object);
    
    EXPECT_EQ(1, region.x);
    EXPECT_EQ(0, region.y);
}

/**
 * @test Объект во второй ячейке по Y
 */
TEST_F(GridRegionResolverTest, ObjectInSecondCellY) {
    GridRegionResolver resolver(100.0);
    setPosition(50, 150);
    
    RegionId region = resolver.getRegion(*object);
    
    EXPECT_EQ(0, region.x);
    EXPECT_EQ(1, region.y);
}

/**
 * @test Объект на границе ячеек (ровно на границе идёт в следующую)
 */
TEST_F(GridRegionResolverTest, ObjectOnBoundary) {
    GridRegionResolver resolver(100.0);
    setPosition(100, 100);
    
    RegionId region = resolver.getRegion(*object);
    
    EXPECT_EQ(1, region.x);
    EXPECT_EQ(1, region.y);
}

/**
 * @test Объект с отрицательными координатами
 */
TEST_F(GridRegionResolverTest, ObjectWithNegativeCoordinates) {
    GridRegionResolver resolver(100.0);
    setPosition(-50, -50);
    
    RegionId region = resolver.getRegion(*object);
    
    EXPECT_EQ(-1, region.x);
    EXPECT_EQ(-1, region.y);
}

/**
 * @test Resolver со смещением
 */
TEST_F(GridRegionResolverTest, ResolverWithOffset) {
    // Смещение на половину ячейки
    GridRegionResolver resolver(100.0, Vector2D(50, 50));
    setPosition(50, 50);  // Без смещения был бы регион (0,0)
    
    RegionId region = resolver.getRegion(*object);
    
    // (50-50)/100 = 0, floor(0) = 0
    EXPECT_EQ(0, region.x);
    EXPECT_EQ(0, region.y);
}

/**
 * @test Смещение меняет регион для граничного объекта
 */
TEST_F(GridRegionResolverTest, OffsetChangesBoundaryRegion) {
    setPosition(99, 99);  // Чуть левее границы
    
    // Без смещения - регион (0, 0)
    GridRegionResolver resolver1(100.0);
    RegionId region1 = resolver1.getRegion(*object);
    EXPECT_EQ(0, region1.x);
    EXPECT_EQ(0, region1.y);
    
    // Со смещением 50 - регион (0, 0) тоже, но (99-50)/100 = 0.49
    GridRegionResolver resolver2(100.0, Vector2D(50, 50));
    RegionId region2 = resolver2.getRegion(*object);
    EXPECT_EQ(0, region2.x);
    EXPECT_EQ(0, region2.y);
}

/**
 * @test Исключение при нулевом размере ячейки
 */
TEST_F(GridRegionResolverTest, ThrowsOnZeroCellSize) {
    EXPECT_THROW(GridRegionResolver(0.0), std::invalid_argument);
}

/**
 * @test Исключение при отрицательном размере ячейки
 */
TEST_F(GridRegionResolverTest, ThrowsOnNegativeCellSize) {
    EXPECT_THROW(GridRegionResolver(-100.0), std::invalid_argument);
}

/**
 * @test Получение размера ячейки
 */
TEST_F(GridRegionResolverTest, GetCellSize) {
    GridRegionResolver resolver(150.0);
    EXPECT_DOUBLE_EQ(150.0, resolver.getCellSize());
}

/**
 * @test Получение смещения
 */
TEST_F(GridRegionResolverTest, GetOffset) {
    GridRegionResolver resolver(100.0, Vector2D(25, 75));
    Vector2D offset = resolver.getOffset();
    
    EXPECT_DOUBLE_EQ(25.0, offset.x);
    EXPECT_DOUBLE_EQ(75.0, offset.y);
}