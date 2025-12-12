#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Order.hpp"
#include "OrderInterpreter.hpp"
#include "UObject.hpp"
#include "IoC.hpp"
#include "ICommand.hpp"
#include "ICommandFactory.hpp"
#include "commands/StartMoveCommandFactory.hpp"
#include "commands/StopMoveCommandFactory.hpp"
#include "commands/FireCommandFactory.hpp"
#include <memory>
#include <functional>

/**
 * @file OrderInterpreterTest.cpp
 * @brief Тесты интерпретатора приказов
 * @author Anton Tobolkin
 */

class OrderInterpreterTest : public ::testing::Test {
protected:
    static bool commandsRegistered_;  // Флаг однократной регистрации команд
    
    void SetUp() override {
        // Регистрируем фабрики команд только один раз
        if (!commandsRegistered_) {
            registerCommandFactory<StartMoveCommandFactory>("StartMove");
            registerCommandFactory<StopMoveCommandFactory>("StopMove");
            registerCommandFactory<FireCommandFactory>("Fire");
            commandsRegistered_ = true;
        }
        
        // Регистрируем тестовый объект
        registerTestObject("basic_ship");
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
    template<typename FactoryType>
    void registerCommandFactory(const std::string& commandName) {
        auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void> {
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<ICommandFactory>(
                        std::make_shared<FactoryType>()
                    )
                );
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

bool OrderInterpreterTest::commandsRegistered_ = false;

/**
 * @brief Тест успешного выполнения StartMove
 */
TEST_F(OrderInterpreterTest, InterpretStartMove_Success)
{
    Order order = createOrder("basic_ship", "StartMove", {{"initialVelocity", 5}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест успешного выполнения StopMove
 */
TEST_F(OrderInterpreterTest, InterpretStopMove_Success)
{
    Order order = createOrder("basic_ship", "StopMove");
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест успешного выполнения Fire
 */
TEST_F(OrderInterpreterTest, InterpretFire_Success)
{
    Order order = createOrder("basic_ship", "Fire", {{"targetId", std::string("enemy_001")}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Тест Fire без цели
 */
TEST_F(OrderInterpreterTest, InterpretFireWithoutTarget_Success)
{
    Order order = createOrder("basic_ship", "Fire");
    
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
    order.objectId = "basic_ship";
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
    Order order = createOrder("basic_ship", "UnknownAction");
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест ошибки StartMove без обязательного параметра
 */
TEST_F(OrderInterpreterTest, InterpretStartMoveWithoutVelocity_ThrowsException)
{
    Order order = createOrder("basic_ship", "StartMove");  // Без initialVelocity
    
    OrderInterpreter interpreter;
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Тест расширяемости: добавление новой команды без изменения интерпретатора
 */
TEST_F(OrderInterpreterTest, ExtensibilityTest_AddNewCommand)
{
    // Создаем фабрику для новой команды "Rotate" динамически
    class RotateCommandFactory : public ICommandFactory {
    public:
        std::shared_ptr<ICommand> create(
            std::shared_ptr<IUObject> object,
            std::shared_ptr<IUObject> params) override
        {
            // Простая команда поворота
            class RotateCommand : public ICommand {
            public:
                RotateCommand(std::shared_ptr<IUObject> obj, std::shared_ptr<IUObject> p)
                    : object_(obj), params_(p) {}
                void execute() override {
                    int angle = std::any_cast<int>(params_->getProperty("angle"));
                    object_->setProperty("rotation", angle);
                }
            private:
                std::shared_ptr<IUObject> object_;
                std::shared_ptr<IUObject> params_;
            };
            return std::make_shared<RotateCommand>(object, params);
        }
    };
    
    // Регистрируем фабрику новой команды
    auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
        []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(
                std::static_pointer_cast<ICommandFactory>(
                    std::make_shared<RotateCommandFactory>()
                )
            );
        }
    );
    
    auto key = std::make_shared<std::string>("Commands.Rotate");
    std::vector<std::shared_ptr<void>> regArgs = {key, factory};
    auto registerCmd = IoC::resolve<ICommand>("IoC.Register", regArgs);
    registerCmd->execute();
    
    // Теперь интерпретатор может обрабатывать Rotate без изменения кода
    Order order = createOrder("basic_ship", "Rotate", {{"angle", 45}});
    
    OrderInterpreter interpreter;
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}