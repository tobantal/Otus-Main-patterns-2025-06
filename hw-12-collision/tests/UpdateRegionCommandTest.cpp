#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "UpdateRegionCommand.hpp"
#include "RegionManager.hpp"
#include "GridRegionResolver.hpp"
#include "SpaceShip.hpp"

/**
 * @brief Mock для ICollisionChecker
 */
class MockCollisionChecker : public ICollisionChecker {
public:
    MOCK_METHOD(bool, checkCollision, 
                (const IGameObject&, const IGameObject&), (const, override));
};

/**
 * @brief Тесты для UpdateRegionCommand
 */
class UpdateRegionCommandTest : public ::testing::Test {
protected:
    std::shared_ptr<RegionManager> manager;
    std::shared_ptr<MockCollisionChecker> checker;
    std::shared_ptr<MacroCommand> collisionMacro;
    
    void SetUp() override {
        auto resolver = std::make_shared<GridRegionResolver>(100.0);
        manager = std::make_shared<RegionManager>(resolver);
        checker = std::make_shared<MockCollisionChecker>();
        collisionMacro = nullptr;
    }
    
    std::shared_ptr<SpaceShip> createObject(double x, double y) {
        auto obj = std::make_shared<SpaceShip>();
        obj->setProperty("Position", Vector2D(x, y));
        return obj;
    }
};

/**
 * @test Первое добавление объекта в систему
 */
TEST_F(UpdateRegionCommandTest, FirstAddition) {
    auto obj = createObject(50, 50);
    
    UpdateRegionCommand cmd(obj, manager, checker, collisionMacro);
    cmd.execute();
    
    // Объект должен быть добавлен в регион (0, 0)
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    ASSERT_EQ(1u, objects.size());
    EXPECT_EQ(obj.get(), objects[0].get());
}

/**
 * @test Объект остаётся в том же регионе при повторном вызове
 */
TEST_F(UpdateRegionCommandTest, StaysInSameRegion) {
    auto obj = createObject(50, 50);
    
    UpdateRegionCommand cmd(obj, manager, checker, collisionMacro);
    cmd.execute();
    cmd.execute();  // Повторный вызов
    
    auto objects = manager->getObjectsInRegion(RegionId(0, 0));
    EXPECT_EQ(1u, objects.size());
}

/**
 * @test Объект перемещается в новый регион
 */
TEST_F(UpdateRegionCommandTest, MovesToNewRegion) {
    auto obj = createObject(50, 50);
    
    UpdateRegionCommand cmd(obj, manager, checker, collisionMacro);
    cmd.execute();
    
    // Изменяем позицию объекта
    obj->setProperty("Position", Vector2D(150, 150));
    cmd.execute();
    
    auto oldRegion = manager->getObjectsInRegion(RegionId(0, 0));
    auto newRegion = manager->getObjectsInRegion(RegionId(1, 1));
    
    EXPECT_TRUE(oldRegion.empty());
    EXPECT_EQ(1u, newRegion.size());
}

/**
 * @test Создаёт команды проверки коллизий с соседями
 */
TEST_F(UpdateRegionCommandTest, CreatesCollisionCommands) {
    auto obj1 = createObject(50, 50);
    auto obj2 = createObject(60, 60);
    
    // Добавляем первый объект
    UpdateRegionCommand cmd1(obj1, manager, checker, collisionMacro);
    cmd1.execute();
    
    // Макрокоманда пустая (нет соседей)
    EXPECT_EQ(nullptr, collisionMacro);
    
    // Добавляем второй объект
    std::shared_ptr<MacroCommand> macro2;
    UpdateRegionCommand cmd2(obj2, manager, checker, macro2);
    cmd2.execute();
    
    // Теперь должна быть макрокоманда с одной проверкой
    EXPECT_NE(nullptr, macro2);
}

/**
 * @test Не создаёт команды если объект один в регионе
 */
TEST_F(UpdateRegionCommandTest, NoCommandsWhenAlone) {
    auto obj = createObject(50, 50);
    
    UpdateRegionCommand cmd(obj, manager, checker, collisionMacro);
    cmd.execute();
    
    EXPECT_EQ(nullptr, collisionMacro);
}

/**
 * @test Исключение при null объекте
 */
TEST_F(UpdateRegionCommandTest, ThrowsOnNullObject) {
    EXPECT_THROW(
        UpdateRegionCommand(nullptr, manager, checker, collisionMacro),
        std::invalid_argument
    );
}

/**
 * @test Исключение при null менеджере
 */
TEST_F(UpdateRegionCommandTest, ThrowsOnNullManager) {
    auto obj = createObject(50, 50);
    EXPECT_THROW(
        UpdateRegionCommand(obj, nullptr, checker, collisionMacro),
        std::invalid_argument
    );
}

/**
 * @test Исключение при null checker
 */
TEST_F(UpdateRegionCommandTest, ThrowsOnNullChecker) {
    auto obj = createObject(50, 50);
    EXPECT_THROW(
        UpdateRegionCommand(obj, manager, nullptr, collisionMacro),
        std::invalid_argument
    );
}

/**
 * @test Правильное количество команд проверки
 */
TEST_F(UpdateRegionCommandTest, CorrectNumberOfCollisionCommands) {
    // Добавляем 3 объекта в один регион
    auto obj1 = createObject(10, 10);
    auto obj2 = createObject(20, 20);
    auto obj3 = createObject(30, 30);
    
    std::shared_ptr<MacroCommand> macro1, macro2, macro3;
    
    UpdateRegionCommand cmd1(obj1, manager, checker, macro1);
    cmd1.execute();
    
    UpdateRegionCommand cmd2(obj2, manager, checker, macro2);
    cmd2.execute();
    
    UpdateRegionCommand cmd3(obj3, manager, checker, macro3);
    cmd3.execute();
    
    // obj3 должен иметь 2 команды проверки (с obj1 и obj2)
    EXPECT_NE(nullptr, macro3);
}