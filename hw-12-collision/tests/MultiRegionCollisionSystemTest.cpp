#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MultiRegionCollisionSystem.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Mock для ICollisionChecker
 */
class MockCollisionChecker2 : public ICollisionChecker {
public:
    MOCK_METHOD(bool, checkCollision, 
                (const IGameObject&, const IGameObject&), (const, override));
};

/**
 * @brief Тесты для MultiRegionCollisionSystem
 */
class MultiRegionCollisionSystemTest : public ::testing::Test {
protected:
    std::shared_ptr<MockCollisionChecker2> checker;
    
    void SetUp() override {
        checker = std::make_shared<MockCollisionChecker2>();
    }
    
    std::shared_ptr<SpaceShip> createObject(double x, double y) {
        auto obj = std::make_shared<SpaceShip>();
        obj->setProperty("Position", Vector2D(x, y));
        return obj;
    }
};

/**
 * @test Создание системы с 2 подсистемами (по умолчанию)
 */
TEST_F(MultiRegionCollisionSystemTest, CreateWithDefaultSystems) {
    MultiRegionCollisionSystem system(100.0, checker);
    
    EXPECT_EQ(2, system.getNumSystems());
}

/**
 * @test Создание системы с произвольным числом подсистем
 */
TEST_F(MultiRegionCollisionSystemTest, CreateWithCustomSystems) {
    MultiRegionCollisionSystem system(100.0, checker, 4);
    
    EXPECT_EQ(4, system.getNumSystems());
}

/**
 * @test Создание с 1 подсистемой
 */
TEST_F(MultiRegionCollisionSystemTest, CreateWithSingleSystem) {
    MultiRegionCollisionSystem system(100.0, checker, 1);
    
    EXPECT_EQ(1, system.getNumSystems());
}

/**
 * @test Исключение при нулевом размере ячейки
 */
TEST_F(MultiRegionCollisionSystemTest, ThrowsOnZeroCellSize) {
    EXPECT_THROW(
        MultiRegionCollisionSystem(0.0, checker),
        std::invalid_argument
    );
}

/**
 * @test Исключение при отрицательном количестве систем
 */
TEST_F(MultiRegionCollisionSystemTest, ThrowsOnZeroSystems) {
    EXPECT_THROW(
        MultiRegionCollisionSystem(100.0, checker, 0),
        std::invalid_argument
    );
}

/**
 * @test Исключение при null checker
 */
TEST_F(MultiRegionCollisionSystemTest, ThrowsOnNullChecker) {
    EXPECT_THROW(
        MultiRegionCollisionSystem(100.0, nullptr),
        std::invalid_argument
    );
}

/**
 * @test Добавление объекта во все подсистемы
 */
TEST_F(MultiRegionCollisionSystemTest, AddObjectToAllSystems) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    auto obj = createObject(50, 50);
    
    system.addObject(obj);
    
    // Проверяем, что объект есть в обеих системах
    auto manager0 = system.getManager(0);
    auto manager1 = system.getManager(1);
    
    RegionId region0 = manager0->getResolver().getRegion(*obj);
    RegionId region1 = manager1->getResolver().getRegion(*obj);
    
    auto objects0 = manager0->getObjectsInRegion(region0);
    auto objects1 = manager1->getObjectsInRegion(region1);
    
    EXPECT_EQ(1u, objects0.size());
    EXPECT_EQ(1u, objects1.size());
}

/**
 * @test Удаление объекта из всех подсистем
 */
TEST_F(MultiRegionCollisionSystemTest, RemoveObjectFromAllSystems) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    auto obj = createObject(50, 50);
    
    system.addObject(obj);
    system.removeObject(obj);
    
    auto manager0 = system.getManager(0);
    auto manager1 = system.getManager(1);
    
    // Проверяем, что объекта нет ни в одной системе
    auto objects0 = manager0->getObjectsInRegion(RegionId(0, 0));
    auto objects1 = manager1->getObjectsInRegion(RegionId(0, 0));
    
    EXPECT_TRUE(objects0.empty());
    EXPECT_TRUE(objects1.empty());
}

/**
 * @test Объекты на границе попадают в один регион во второй системе
 * 
 * Это ключевой тест для проверки решения проблемы границ
 */
TEST_F(MultiRegionCollisionSystemTest, BoundaryObjectsInSameRegionInSecondSystem) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    
    // Объект 1: слева от границы (99, 50) -> регион (0, 0) в системе 0
    auto obj1 = createObject(99, 50);
    
    // Объект 2: справа от границы (101, 50) -> регион (1, 0) в системе 0
    auto obj2 = createObject(101, 50);
    
    system.addObject(obj1);
    system.addObject(obj2);
    
    // В системе 0 они в разных регионах
    auto manager0 = system.getManager(0);
    RegionId region1_sys0 = manager0->getResolver().getRegion(*obj1);
    RegionId region2_sys0 = manager0->getResolver().getRegion(*obj2);
    
    EXPECT_NE(region1_sys0, region2_sys0);  // Разные регионы
    
    // В системе 1 (смещение 50) они в одном регионе
    // obj1: (99-50)/100 = 0.49 -> регион 0
    // obj2: (101-50)/100 = 0.51 -> регион 0
    auto manager1 = system.getManager(1);
    RegionId region1_sys1 = manager1->getResolver().getRegion(*obj1);
    RegionId region2_sys1 = manager1->getResolver().getRegion(*obj2);
    
    EXPECT_EQ(region1_sys1, region2_sys1);  // Один регион!
}

/**
 * @test Получение менеджера по индексу
 */
TEST_F(MultiRegionCollisionSystemTest, GetManagerByIndex) {
    MultiRegionCollisionSystem system(100.0, checker, 3);
    
    EXPECT_NO_THROW(system.getManager(0));
    EXPECT_NO_THROW(system.getManager(1));
    EXPECT_NO_THROW(system.getManager(2));
}

/**
 * @test Исключение при неверном индексе менеджера
 */
TEST_F(MultiRegionCollisionSystemTest, ThrowsOnInvalidManagerIndex) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    
    EXPECT_THROW(system.getManager(-1), std::out_of_range);
    EXPECT_THROW(system.getManager(2), std::out_of_range);
    EXPECT_THROW(system.getManager(100), std::out_of_range);
}

/**
 * @test Обновление объекта генерирует команды коллизий
 */
TEST_F(MultiRegionCollisionSystemTest, UpdateObjectGeneratesCommands) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    
    auto obj1 = createObject(50, 50);
    auto obj2 = createObject(60, 60);
    
    system.addObject(obj1);
    system.addObject(obj2);
    
    // Обновляем obj2 - должны появиться команды коллизий
    system.updateObject(obj2);
    
    auto commands = system.getCollisionCommands();
    
    // Должны быть команды (из одной или обеих систем)
    EXPECT_FALSE(commands.empty());
}

/**
 * @test Пустой список команд когда объект один
 */
TEST_F(MultiRegionCollisionSystemTest, NoCommandsWhenAlone) {
    MultiRegionCollisionSystem system(100.0, checker, 2);
    
    auto obj = createObject(50, 50);
    system.addObject(obj);
    system.updateObject(obj);
    
    auto commands = system.getCollisionCommands();
    
    // Нет соседей - нет команд
    EXPECT_TRUE(commands.empty());
}

/**
 * @test Система с 4 подсистемами
 */
TEST_F(MultiRegionCollisionSystemTest, FourSystemsWork) {
    MultiRegionCollisionSystem system(100.0, checker, 4);
    
    auto obj = createObject(50, 50);
    system.addObject(obj);
    
    // Проверяем все 4 менеджера
    for (int i = 0; i < 4; ++i) {
        auto manager = system.getManager(i);
        RegionId region = manager->getResolver().getRegion(*obj);
        auto objects = manager->getObjectsInRegion(region);
        
        EXPECT_EQ(1u, objects.size()) << "System " << i << " failed";
    }
}

/**
 * @test Проверка разных смещений в многосистемной конфигурации
 */
TEST_F(MultiRegionCollisionSystemTest, DifferentOffsetsInMultipleSystems) {
    MultiRegionCollisionSystem system(100.0, checker, 4);
    
    // Проверяем, что смещения разные
    auto manager0 = system.getManager(0);
    auto manager1 = system.getManager(1);
    auto manager2 = system.getManager(2);
    auto manager3 = system.getManager(3);
    
    // Смещения должны быть: 0, 25, 50, 75 (для cellSize=100, N=4)
    // Проверим это косвенно через объект на позиции (25, 25)
    auto obj = createObject(25, 25);
    
    RegionId r0 = manager0->getResolver().getRegion(*obj);
    RegionId r1 = manager1->getResolver().getRegion(*obj);
    RegionId r2 = manager2->getResolver().getRegion(*obj);
    RegionId r3 = manager3->getResolver().getRegion(*obj);
    
    // (25-0)/100 = 0.25 -> 0
    EXPECT_EQ(0, r0.x);
    
    // (25-25)/100 = 0 -> 0
    EXPECT_EQ(0, r1.x);
    
    // (25-50)/100 = -0.25 -> -1
    EXPECT_EQ(-1, r2.x);
    
    // (25-75)/100 = -0.5 -> -1
    EXPECT_EQ(-1, r3.x);
}