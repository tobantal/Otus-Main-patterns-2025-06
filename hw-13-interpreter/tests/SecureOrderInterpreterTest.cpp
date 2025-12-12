#include <gtest/gtest.h>
#include "Order.hpp"
#include "SecureOrderInterpreter.hpp"
#include "UObject.hpp"
#include "IoC.hpp"
#include "ICommand.hpp"
#include "ICommandFactory.hpp"
#include "commands/StartMoveCommandFactory.hpp"
#include "commands/StopMoveCommandFactory.hpp"
#include "commands/FireCommandFactory.hpp"
#include <memory>
#include <functional>
#include <sstream>

/**
 * @file SecureOrderInterpreterTest.cpp
 * @brief Тесты защиты от чужих приказов через скоупы
 * @author Anton Tobolkin
 */

class SecureOrderInterpreterTest : public ::testing::Test {
protected:
    static int testCounter_;  // Счётчик для уникальных имён
    static bool commandsRegistered_;  // Флаг однократной регистрации команд
    std::string testPrefix_;   // Уникальный префикс для скоупов игроков
    
    void SetUp() override {
        // Регистрируем глобальные фабрики команд только один раз (при первом тесте)
        if (!commandsRegistered_) {
            registerGlobalCommandFactory<StartMoveCommandFactory>("StartMove");
            registerGlobalCommandFactory<StopMoveCommandFactory>("StopMove");
            registerGlobalCommandFactory<FireCommandFactory>("Fire");
            commandsRegistered_ = true;
        }
        
        // Создаём уникальный префикс для этого теста
        std::stringstream ss;
        ss << "SecTest" << testCounter_++ << "_";
        testPrefix_ = ss.str();
        
        // Создаем скоупы для игроков с уникальными именами
        createPlayerScope("Alice");
        createPlayerScope("Bob");
        
        // Регистрируем объекты в скоупах игроков
        registerObjectInPlayerScope("Alice", "ship_001");
        registerObjectInPlayerScope("Alice", "ship_002");
        registerObjectInPlayerScope("Bob", "ship_003");
        registerObjectInPlayerScope("Bob", "ship_004");
    }
    
    /**
     * @brief Возвращает полное имя скоупа игрока (с учётом уникального префикса)
     */
    std::string getPlayerScopeId(const std::string& playerId) {
        return "Player." + testPrefix_ + playerId;
    }
    
    /**
     * @brief Создает скоуп для игрока
     */
    void createPlayerScope(const std::string& playerId) {
        auto scopeId = std::make_shared<std::string>(getPlayerScopeId(playerId));
        std::vector<std::shared_ptr<void>> args = {scopeId};
        
        auto createCommand = IoC::resolve<ICommand>("Scopes.New", args);
        createCommand->execute();
    }
    
    /**
     * @brief Регистрирует объект в скоупе игрока
     */
    void registerObjectInPlayerScope(
        const std::string& playerId, 
        const std::string& objectId) 
    {
        // Переключаемся на скоуп игрока
        auto scopeId = std::make_shared<std::string>(getPlayerScopeId(playerId));
        std::vector<std::shared_ptr<void>> scopeArgs = {scopeId};
        auto setCommand = IoC::resolve<ICommand>("Scopes.Current", scopeArgs);
        setCommand->execute();
        
        // Регистрируем объект в текущем скоупе
        auto factory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            [objectId, playerId]() -> std::shared_ptr<void> {
                auto obj = std::make_shared<UObject>();
                obj->setProperty("id", objectId);
                obj->setProperty("ownerId", playerId);
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<IUObject>(obj)
                );
            }
        );
        
        auto key = std::make_shared<std::string>("Objects." + objectId);
        std::vector<std::shared_ptr<void>> regArgs = {key, factory};
        
        auto registerCmd = IoC::resolve<ICommand>("IoC.Register", regArgs);
        registerCmd->execute();
    }
    
    /**
     * @brief Регистрирует глобальную фабрику команды
     * 
     * Вызывается в начале SetUp когда скоуп не установлен.
     */
    template<typename FactoryType>
    void registerGlobalCommandFactory(const std::string& commandName) {
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
     * @brief Создает SecureOrderInterpreter для игрока (с учётом уникального префикса)
     */
    SecureOrderInterpreter createInterpreter(const std::string& playerId) {
        return SecureOrderInterpreter(testPrefix_ + playerId);
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

int SecureOrderInterpreterTest::testCounter_ = 0;
bool SecureOrderInterpreterTest::commandsRegistered_ = false;

// ============================================================================
// Тесты доступа Alice к своим кораблям
// ============================================================================

/**
 * @brief Alice может управлять своим кораблем ship_001
 */
TEST_F(SecureOrderInterpreterTest, AliceCanControlOwnShip001)
{
    Order order = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    
    auto interpreter = createInterpreter("Alice");
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Alice может управлять своим кораблем ship_002
 */
TEST_F(SecureOrderInterpreterTest, AliceCanControlOwnShip002)
{
    Order order = createOrder("ship_002", "StopMove");
    
    auto interpreter = createInterpreter("Alice");
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Alice может стрелять из своего корабля
 */
TEST_F(SecureOrderInterpreterTest, AliceCanFireFromOwnShip)
{
    Order order = createOrder("ship_001", "Fire", {{"targetId", std::string("enemy")}});
    
    auto interpreter = createInterpreter("Alice");
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

// ============================================================================
// Тесты запрета доступа Alice к чужим кораблям
// ============================================================================

/**
 * @brief Alice НЕ может управлять кораблем Bob'а ship_003
 */
TEST_F(SecureOrderInterpreterTest, AliceCannotControlBobsShip003)
{
    Order order = createOrder("ship_003", "StartMove", {{"initialVelocity", 5}});
    
    auto interpreter = createInterpreter("Alice");
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Alice НЕ может управлять кораблем Bob'а ship_004
 */
TEST_F(SecureOrderInterpreterTest, AliceCannotControlBobsShip004)
{
    Order order = createOrder("ship_004", "StopMove");
    
    auto interpreter = createInterpreter("Alice");
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

// ============================================================================
// Тесты доступа Bob к своим кораблям
// ============================================================================

/**
 * @brief Bob может управлять своим кораблем ship_003
 */
TEST_F(SecureOrderInterpreterTest, BobCanControlOwnShip003)
{
    Order order = createOrder("ship_003", "StartMove", {{"initialVelocity", 3}});
    
    auto interpreter = createInterpreter("Bob");
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

/**
 * @brief Bob может управлять своим кораблем ship_004
 */
TEST_F(SecureOrderInterpreterTest, BobCanControlOwnShip004)
{
    Order order = createOrder("ship_004", "Fire", {{"targetId", std::string("target")}});
    
    auto interpreter = createInterpreter("Bob");
    
    EXPECT_NO_THROW(interpreter.interpret(order));
}

// ============================================================================
// Тесты запрета доступа Bob к чужим кораблям
// ============================================================================

/**
 * @brief Bob НЕ может управлять кораблем Alice ship_001
 */
TEST_F(SecureOrderInterpreterTest, BobCannotControlAlicesShip001)
{
    Order order = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    
    auto interpreter = createInterpreter("Bob");
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

/**
 * @brief Bob НЕ может управлять кораблем Alice ship_002
 */
TEST_F(SecureOrderInterpreterTest, BobCannotControlAlicesShip002)
{
    Order order = createOrder("ship_002", "StopMove");
    
    auto interpreter = createInterpreter("Bob");
    
    EXPECT_THROW(interpreter.interpret(order), std::runtime_error);
}

// ============================================================================
// Тесты множественных операций
// ============================================================================

/**
 * @brief Игрок может управлять несколькими своими кораблями подряд
 */
TEST_F(SecureOrderInterpreterTest, PlayerCanControlMultipleOwnShips)
{
    auto interpreter = createInterpreter("Alice");
    
    Order order1 = createOrder("ship_001", "StartMove", {{"initialVelocity", 5}});
    Order order2 = createOrder("ship_002", "StopMove");
    Order order3 = createOrder("ship_001", "Fire");
    
    EXPECT_NO_THROW(interpreter.interpret(order1));
    EXPECT_NO_THROW(interpreter.interpret(order2));
    EXPECT_NO_THROW(interpreter.interpret(order3));
}

/**
 * @brief Проверка сообщения об ошибке при попытке доступа к чужому кораблю
 */
TEST_F(SecureOrderInterpreterTest, AccessDeniedErrorMessage)
{
    Order order = createOrder("ship_003", "StartMove", {{"initialVelocity", 5}});
    
    auto interpreter = createInterpreter("Alice");
    
    try {
        interpreter.interpret(order);
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        std::string errorMsg = e.what();
        // Проверяем, что сообщение содержит информацию об объекте
        // (объект не найден в скоупе = отказ в доступе)
        EXPECT_TRUE(errorMsg.find("ship_003") != std::string::npos ||
                    errorMsg.find("not found") != std::string::npos ||
                    errorMsg.find("Object") != std::string::npos);
    }
}

/**
 * @brief Тест получения ID игрока
 */
TEST_F(SecureOrderInterpreterTest, GetPlayerId)
{
    auto interpreter = createInterpreter("Alice");
    
    // playerId содержит уникальный префикс теста
    EXPECT_TRUE(interpreter.getPlayerId().find("Alice") != std::string::npos);
}