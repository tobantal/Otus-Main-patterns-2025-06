#include <gtest/gtest.h>
#include "RegionManager.hpp"
#include "GridRegionResolver.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Тесты для RegionManager
 */
class RegionManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<RegionManager> manager;
    
    void SetUp() override {
        auto resolver = std::make_shared<GridRegionResolver>(100.0);
        manager = std::make_shared<RegionManager>(resolver);
    }
    
    std::shared_ptr<SpaceShip> createObject(double x, double y) {
        auto obj = std::make_shared<SpaceShip>();
        obj->setProperty("Position", Vector2D(x, y));
        return obj;
    }
};

/**
 * @test Добавление объекта в регион
 */
TEST_F(RegionManagerTest, AddObject) {
    auto obj = createObject(50, 50);
    
    manager->addObject(obj);
    
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    ASSERT_EQ(1u, objects.size());
    EXPECT_EQ(obj.get(), objects[0].get());
}

/**
 * @test Добавление нескольких объектов в один регион
 */
TEST_F(RegionManagerTest, AddMultipleObjectsToSameRegion) {
    auto obj1 = createObject(10, 10);
    auto obj2 = createObject(90, 90);
    
    manager->addObject(obj1);
    manager->addObject(obj2);
    
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    EXPECT_EQ(2u, objects.size());
}

/**
 * @test Добавление объектов в разные регионы
 */
TEST_F(RegionManagerTest, AddObjectsToDifferentRegions) {
    auto obj1 = createObject(50, 50);    // Регион (0, 0)
    auto obj2 = createObject(150, 150);  // Регион (1, 1)
    
    manager->addObject(obj1);
    manager->addObject(obj2);
    
    auto region00 = manager->getObjectsInRegion(RegionId(0, 0));
    auto region11 = manager->getObjectsInRegion(RegionId(1, 1));
    
    EXPECT_EQ(1u, region00.size());
    EXPECT_EQ(1u, region11.size());
    EXPECT_EQ(2u, manager->getRegionCount());
}

/**
 * @test Удаление объекта из региона
 */
TEST_F(RegionManagerTest, RemoveObject) {
    auto obj = createObject(50, 50);
    
    manager->addObject(obj);
    manager->removeObject(obj);
    
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    EXPECT_TRUE(objects.empty());
}

/**
 * @test Перемещение объекта между регионами
 */
TEST_F(RegionManagerTest, MoveObjectBetweenRegions) {
    auto obj = createObject(50, 50);
    
    manager->addObject(obj);
    
    // Перемещаем из (0,0) в (1,1)
    manager->moveObject(obj, RegionId(0, 0), RegionId(1, 1));
    
    auto oldRegion = manager->getObjectsInRegion(RegionId(0, 0));
    auto newRegion = manager->getObjectsInRegion(RegionId(1, 1));
    
    EXPECT_TRUE(oldRegion.empty());
    EXPECT_EQ(1u, newRegion.size());
}

/**
 * @test Перемещение в тот же регион (ничего не происходит)
 */
TEST_F(RegionManagerTest, MoveObjectToSameRegion) {
    auto obj = createObject(50, 50);
    
    manager->addObject(obj);
    manager->moveObject(obj, RegionId(0, 0), RegionId(0, 0));
    
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    EXPECT_EQ(1u, objects.size());
}

/**
 * @test Получение объектов из пустого региона
 */
TEST_F(RegionManagerTest, GetObjectsFromEmptyRegion) {
    auto objects = manager->getObjectsInRegion(RegionId(99, 99));
    EXPECT_TRUE(objects.empty());
}

/**
 * @test Исключение при null resolver
 */
TEST_F(RegionManagerTest, ThrowsOnNullResolver) {
    EXPECT_THROW(RegionManager(nullptr), std::invalid_argument);
}

/**
 * @test Исключение при добавлении null объекта
 */
TEST_F(RegionManagerTest, ThrowsOnNullObject) {
    EXPECT_THROW(manager->addObject(nullptr), std::invalid_argument);
}

/**
 * @test Удаление null объекта (не должно бросать исключение)
 */
TEST_F(RegionManagerTest, RemoveNullObjectDoesNotThrow) {
    EXPECT_NO_THROW(manager->removeObject(nullptr));
}

/**
 * @test Получение resolver
 */
TEST_F(RegionManagerTest, GetResolver) {
    const IRegionResolver& resolver = manager->getResolver();
    EXPECT_DOUBLE_EQ(100.0, resolver.getCellSize());
}