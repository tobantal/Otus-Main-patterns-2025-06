#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "OrderInterpreter.hpp"
#include "UObject.hpp"
#include "IoC.hpp"
#include "ICommand.hpp"
#include "commands/StartMoveCommand.hpp"
#include "commands/StopMoveCommand.hpp"
#include "commands/FireCommand.hpp"
#include <memory>
#include <functional>

/**
 * @file OrderInterpreterTest.cpp
 * @brief Тесты интерпретатора приказов
 * @author Anton Tobolkin
 */

class OrderInterpreterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Регистрируем тестовый объект
        registerTestObject("ship_001");
        
        // Регистрируем команды
        registerCommand("StartMove");
        registerCommand("StopMove");
        registerCommand("Fire");
    }
    
    /**
     * @brief Регистрирует тестовый объект в IoC
     */
    void registerTestObject(const std::string& objectId) {
        auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            [objectId]() -> std::shared_ptr<void> {
                auto obj = std::make_shared<UObject>();
                obj->setProperty("id", objectId);
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<IUObject>(obj)
                );
            }
        );
        
        auto key = std::make_shared<std::string>("Objects." + objectId);
        std::vector<std::shared_ptr<void>> args = {key, factory};
        
        auto registerCommand = IoC::resolve<ICommand>("IoC.Register", args);
        registerCommand->execute();
    }
    
    /**
     * @brief Регистрирует фабрику команды в IoC
     */
    void registerCommand(const std::string& commandName) {
        // Фабрика с параметрами: (object, params)
        auto factory = std::make_shared<std::function<std::shared_ptr<void>(std::vector<std::shared_ptr<void>>)>>(
            [commandName](std::vector<std::shared_ptr<void>> args) -> std::shared_ptr<void> {
                auto object = std::static_pointer_cast<IUObject>(args[0]);
                auto params = std::static_pointer_cast<IUObject>(args[1]);
                
                std::shared_ptr<ICommand> cmd;
                if (commandName == "StartMove") {
                    cmd = std::make_shared<StartMoveCommand>(object, params);
                } else if (commandName == "StopMove") {
                    cmd = std::make_shared<StopMoveCommand>(object, params);
                } else if (commandName == "Fire") {
                    cmd = std::make_shared<FireCommand>(object, params);
                }
                
                return std::static_pointer_cast<void>(cmd);
            }
        );
        
        auto key = std::make_shared<std::string>("Commands." + commandName);
        std::vector<std::shared_ptr<void>> args = {key, factory};
        
        auto registerCmd = IoC::resolve<ICommand>("IoC.Register", args);
        registerCmd->execute();
    }
    
    /**
     * @brief Создает Order для тестов
     */
    Order createOrder(
        const std::string& objectId,
        const std::string& action,
        std::initializer_list<std::pair<std::string, std::any>> params = {})
    {
        Order order;
        order.objectId = objectId;
        order.action = action;
        
        auto uobj = std::make_shared<UObject>();
        for (const auto& [key, value] : params) {
            uobj->setProperty(key, value);
        }
        order.parameters = uobj;
        
        return order;
    }
};

/**
 * @brief Тест успешного выполнения StartMove
 */
TEST_F(OrderInterpreterTest, InterpretStartMove_Success)
{
    Order order = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест успешного выполнения StopMove
 */
TEST_F(OrderInterpreterTest, InterpretStopMove_Success)
{
    Order order = createOrder("ship_001", "StopMove");
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест успешного выполнения Fire
 */
TEST_F(OrderInterpreterTest, InterpretFire_Success)
{
    Order order = createOrder("ship_001", "Fire", {{"targetId", std::string("enemy_001")}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест Fire без цели
 */
TEST_F(OrderInterpreterTest, InterpretFireWithoutTarget_Success)
{
    Order order = createOrder("ship_001", "Fire");
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест ошибки при невалидном приказе (пустой id)
 */
TEST_F(OrderInterpreterTest, InterpretInvalidOrder_EmptyId_ThrowsException)
{
    Order order;
    order.objectId = "";
    order.action = "StartMove";
    order.parameters = std::make_shared<UObject>();
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест ошибки при невалидном приказе (пустой action)
 */
TEST_F(OrderInterpreterTest, InterpretInvalidOrder_EmptyAction_ThrowsException)
{
    Order order;
    order.objectId = "ship_001";
    order.action = "";
    order.parameters = std::make_shared<UObject>();
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест ошибки при неизвестном объекте
 */
TEST_F(OrderInterpreterTest, InterpretUnknownObject_ThrowsException)
{
    Order order = createOrder("unknown_ship", "StartMove", {{"initialVelocity", 5}});
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест ошибки при неизвестном действии
 */
TEST_F(OrderInterpreterTest, InterpretUnknownAction_ThrowsException)
{
    Order order = createOrder("ship_001", "UnknownAction");
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест ошибки StartMove без обязательного параметра
 */
TEST_F(OrderInterpreterTest, InterpretStartMoveWithoutVelocity_ThrowsException)
{
    Order order = createOrder("ship_001", "StartMove");  // Без initialVelocity
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест расширяемости: добавление новой команды без изменения интерпретатора
 */
TEST_F(OrderInterpreterTest, ExtensibilityTest_AddNewCommand)
{
    // Регистрируем новую команду "Rotate" динамически
    auto factory = std::make_shared<std::function<std::shared_ptr<void>(std::vector<std::shared_ptr<void>>)>>(
        [](std::vector<std::shared_ptr<void>> args) -> std::shared_ptr<void> {
            auto object = std::static_pointer_cast<IUObject>(args[0]);
            auto params = std::static_pointer_cast<IUObject>(args[1]);
            
            // Простая команда поворота
            int angle = std::any_cast<int>(params->getProperty("angle"));
            object->setProperty("rotation", angle);
            
            // Возвращаем команду-заглушку
            class RotateCommand : public ICommand {
            public:
                void execute() override {}
            };
            return std::static_pointer_cast<void>(
                std::static_pointer_cast<ICommand>(std::make_shared<RotateCommand>())
            );
        }
    );
    
    auto key = std::make_shared<std::string>("Commands.Rotate");
    std::vector<std::shared_ptr<void>> regArgs = {key, factory};
    auto registerCmd = IoC::resolve<ICommand>("IoC.Register", regArgs);
    registerCmd->execute();
    
    // Теперь интерпретатор может обрабатывать Rotate без изменения кода
    Order order = createOrder("ship_001", "Rotate", {{"angle", 45}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}