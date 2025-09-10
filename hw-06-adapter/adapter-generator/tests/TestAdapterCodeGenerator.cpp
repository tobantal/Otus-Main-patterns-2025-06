/**
 * @file TestAdapterCodeGenerator.cpp
 * @brief Unit-тесты для AdapterCodeGenerator
 * @author Anton Tobolkin
 * @version 1.0
 */

#include <gtest/gtest.h>
#include "../include/generators/AdapterCodeGenerator.hpp"
#include "../include/models/InterfaceInfo.hpp"
#include "../include/models/MethodInfo.hpp"

/**
 * @brief Тестовый класс для AdapterCodeGenerator
 */
class AdapterCodeGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<AdapterCodeGenerator>();
        setupTestInterface();
    }

    void TearDown() override {
        generator.reset();
    }

    void setupTestInterface() {
        testInterface.className = "IMovable";
        testInterface.namespaceName = "";
        
        // Добавляем getter метод
        MethodInfo getPosition;
        getPosition.name = "getPosition";
        getPosition.returnType = "Vector";
        getPosition.isConst = true;
        getPosition.isPureVirtual = true;
        testInterface.methods.push_back(getPosition);
        
        // Добавляем setter метод
        MethodInfo setPosition;
        setPosition.name = "setPosition";
        setPosition.returnType = "void";
        setPosition.parameters = {"const Vector& pos"};
        setPosition.isPureVirtual = true;
        testInterface.methods.push_back(setPosition);
        
        // Добавляем void метод
        MethodInfo finish;
        finish.name = "finish";
        finish.returnType = "void";
        finish.isPureVirtual = true;
        testInterface.methods.push_back(finish);
    }

    std::unique_ptr<AdapterCodeGenerator> generator;
    InterfaceInfo testInterface;
};

/**
 * @brief Тест генерации базового адаптера
 */
TEST_F(AdapterCodeGeneratorTest, GenerateBasicAdapter) {
    std::string result = generator->generateAdapter(testInterface);
    
    // Проверяем основную структуру класса
    EXPECT_NE(result.find("class MovableAdapter"), std::string::npos);
    EXPECT_NE(result.find("public IMovable"), std::string::npos);
    EXPECT_NE(result.find("std::shared_ptr<IGameObject> m_gameObject;"), std::string::npos);
    EXPECT_NE(result.find("explicit MovableAdapter(std::shared_ptr<IGameObject> gameObject)"), std::string::npos);
    
    // Проверяем наличие всех методов
    EXPECT_NE(result.find("Vector getPosition()"), std::string::npos);
    EXPECT_NE(result.find("void setPosition("), std::string::npos);
    EXPECT_NE(result.find("void finish()"), std::string::npos);
}

/**
 * @brief Тест генерации getter метода
 */
TEST_F(AdapterCodeGeneratorTest, GenerateGetterMethod) {
    std::string result = generator->generateAdapter(testInterface);

    // Проверяем getter метод
    EXPECT_NE(result.find("Vector getPosition() const override"), std::string::npos);
    EXPECT_NE(result.find("IoC::resolve<Vector>(\"IMovable:Position.get\", m_gameObject)"), std::string::npos);
}

/**
 * @brief Тест генерации setter метода
 */
TEST_F(AdapterCodeGeneratorTest, GenerateSetterMethod) {
    std::string result = generator->generateAdapter(testInterface);
    
    // Проверяем setter метод
    EXPECT_NE(result.find("void setPosition(const Vector& value) override"), std::string::npos);
    EXPECT_NE(result.find("IoC::resolve<ICommand>(\"IMovable:Position.set\""), std::string::npos);
    EXPECT_NE(result.find("m_gameObject, std::make_shared<Vector>(value)"), std::string::npos);
    EXPECT_NE(result.find("command->execute()"), std::string::npos);
}

/**
 * @brief Тест генерации void метода
 */
TEST_F(AdapterCodeGeneratorTest, GenerateVoidMethod) {
    std::string result = generator->generateAdapter(testInterface);
    
    // Проверяем void метод
    EXPECT_NE(result.find("void finish() override"), std::string::npos);
    EXPECT_NE(result.find("command = IoC::resolve<ICommand>(\"IMovable:finish\", m_gameObject)"), std::string::npos);
    EXPECT_NE(result.find("command->execute()"), std::string::npos);
}

/**
 * @brief Тест генерации адаптера с namespace
 */
TEST_F(AdapterCodeGeneratorTest, GenerateAdapterWithNamespace) {
    testInterface.namespaceName = "SpaceShip::Operations";
    
    std::string result = generator->generateAdapter(testInterface);
    
    // Проверяем использование полного имени интерфейса
    EXPECT_NE(result.find("public SpaceShip::Operations::IMovable"), std::string::npos);
    
    // Проверяем, что в IoC вызовах используется полное имя
    EXPECT_NE(result.find("\"SpaceShip::Operations::IMovable:Position.get\""), std::string::npos);
}

/**
 * @brief Тест генерации адаптера с кастомными реализациями
 */
TEST_F(AdapterCodeGeneratorTest, GenerateAdapterWithCustomImplementation) {
    // Добавляем метод с кастомной реализацией
    MethodInfo customMethod;
    customMethod.name = "getVelocity";
    customMethod.returnType = "Vector";
    customMethod.isConst = true;
    customMethod.isPureVirtual = true;
    customMethod.customImpl.hasCustom = true;
    customMethod.customImpl.className = "MovableImplementations";
    customMethod.customImpl.methodName = "calculateVelocity";
    customMethod.customImpl.fullReference = "MovableImplementations::calculateVelocity";
    
    testInterface.methods.push_back(customMethod);
    
    std::string result = generator->generateAdapter(testInterface);

    // Проверяем использование кастомной реализации
    EXPECT_NE(result.find("Vector getVelocity() const override"), std::string::npos);
    EXPECT_NE(result.find("MovableImplementations::calculateVelocity(m_gameObject)"), std::string::npos);
    
    // Не должно быть обычного IoC вызова для кастомного метода
    EXPECT_EQ(result.find("IoC::resolve<Vector>(\"IMovable:Velocity.get\""), std::string::npos);
}

/**
 * @brief Тест генерации кода регистрации для одного интерфейса
 */
TEST_F(AdapterCodeGeneratorTest, GenerateRegistrationCodeSingleInterface) {
    std::vector<InterfaceInfo> interfaces = {testInterface};
    
    std::string result = generator->generateRegistrationCode(interfaces);
    
    // Проверяем основную структуру
    EXPECT_NE(result.find("class AdapterRegistration"), std::string::npos);
    EXPECT_NE(result.find("static void registerAll()"), std::string::npos);
    
    // Проверяем регистрацию конкретного адаптера
    EXPECT_NE(result.find("IoC::resolve(\"IoC.Register\""), std::string::npos);
    EXPECT_NE(result.find("\"Adapter:IMovable\""), std::string::npos);
    EXPECT_NE(result.find("std::make_shared<MovableAdapter>"), std::string::npos);
}

/**
 * @brief Тест генерации кода регистрации для нескольких интерфейсов
 */
TEST_F(AdapterCodeGeneratorTest, GenerateRegistrationCodeMultipleInterfaces) {
    // Создаем второй интерфейс
    InterfaceInfo secondInterface;
    secondInterface.className = "IRotatable";
    secondInterface.namespaceName = "";
    
    MethodInfo getDirection;
    getDirection.name = "getDirection";
    getDirection.returnType = "int";
    getDirection.isConst = true;
    getDirection.isPureVirtual = true;
    secondInterface.methods.push_back(getDirection);
    
    std::vector<InterfaceInfo> interfaces = {testInterface, secondInterface};
    
    std::string result = generator->generateRegistrationCode(interfaces);
    
    // Проверяем регистрацию обоих адаптеров
    EXPECT_NE(result.find("\"Adapter:IMovable\""), std::string::npos);
    EXPECT_NE(result.find("\"Adapter:IRotatable\""), std::string::npos);
    EXPECT_NE(result.find("std::make_shared<MovableAdapter>"), std::string::npos);
    EXPECT_NE(result.find("std::make_shared<RotatableAdapter>"), std::string::npos);
}

/**
 * @brief Тест генерации адаптера для интерфейса без методов
 */
TEST_F(AdapterCodeGeneratorTest, GenerateAdapterForEmptyInterface) {
    InterfaceInfo emptyInterface;
    emptyInterface.className = "IEmpty";
    emptyInterface.namespaceName = "";
    // Нет методов
    
    std::string result = generator->generateAdapter(emptyInterface);
    
    // Основная структура должна быть создана
    EXPECT_NE(result.find("class EmptyAdapter"), std::string::npos);
    EXPECT_NE(result.find("public IEmpty"), std::string::npos);
    
    // Должен быть только деструктор с override, но не методы интерфейса
    // Считаем количество override - должно быть только 1 (деструктор)
    size_t override_count = 0;
    size_t pos = 0;
    while ((pos = result.find("override", pos)) != std::string::npos) {
        override_count++;
        pos += 8; // длина "override"
    }
    
    EXPECT_EQ(override_count, 1); // Только деструктор
    
    // Или более точная проверка - нет методов интерфейса
    // (методы интерфейса обычно не являются деструкторами)
    EXPECT_EQ(result.find(") override;"), std::string::npos); // Нет методов с override и ;
}


/**
 * @brief Тест генерации адаптера с комплексными типами параметров
 */
TEST_F(AdapterCodeGeneratorTest, GenerateAdapterWithComplexParameters) {
    InterfaceInfo complexInterface;
    complexInterface.className = "IComplexInterface";
    complexInterface.namespaceName = "";
    
    // Метод с комплексными параметрами
    MethodInfo complexMethod;
    complexMethod.name = "processData";
    complexMethod.returnType = "void";
    complexMethod.parameters = {
        "const std::vector<std::shared_ptr<Object>>& objects",
        "std::function<bool(const Object&)> predicate",
        "int flags"
    };
    complexMethod.isPureVirtual = true;
    complexInterface.methods.push_back(complexMethod);
    
    std::string result = generator->generateAdapter(complexInterface);
    
    // Проверяем корректную генерацию сигнатуры метода
    EXPECT_NE(result.find("void processData("), std::string::npos);
    EXPECT_NE(result.find("const std::vector<std::shared_ptr<Object>>& objects"), std::string::npos);
    EXPECT_NE(result.find("std::function<bool(const Object&)> predicate"), std::string::npos);
    EXPECT_NE(result.find("int flags"), std::string::npos);
    
    // Проверяем IoC вызов
    EXPECT_NE(result.find("IoC::resolve<ICommand>(\"IComplexInterface:processData\""), std::string::npos);
    EXPECT_NE(result.find("m_gameObject, objects, predicate, flags"), std::string::npos);
}

/**
 * @brief Тест обработки специальных символов в именах
 */
TEST_F(AdapterCodeGeneratorTest, HandleSpecialCharactersInNames) {
    testInterface.className = "ISpecial_Interface";
    
    std::string result = generator->generateAdapter(testInterface);
    
    // Имя адаптера должно быть корректно сформировано
    EXPECT_NE(result.find("class Special_InterfaceAdapter"), std::string::npos);
}

/**
 * @brief Тест генерации include директив
 */
TEST_F(AdapterCodeGeneratorTest, GenerateIncludes) {
    InterfaceInfo secondInterface;
    secondInterface.className = "IRotatable";
    secondInterface.namespaceName = "Different::Namespace";
    
    std::vector<InterfaceInfo> interfaces = {testInterface, secondInterface};
    
    std::string result = generator->generateRegistrationCode(interfaces);
    
    // Проверяем наличие стандартных include'ов
    EXPECT_NE(result.find("#include \"IoC.hpp\""), std::string::npos);
    EXPECT_NE(result.find("#include \"ICommand.hpp\""), std::string::npos);
    
    // Проверяем include'ы для интерфейсов
    EXPECT_NE(result.find("#include \"IMovable.hpp\""), std::string::npos);
    EXPECT_NE(result.find("#include \"IRotatable.hpp\""), std::string::npos);
}

/**
 * @brief Тест устойчивости к некорректным данным
 */
TEST_F(AdapterCodeGeneratorTest, HandleIncorrectData) {
    InterfaceInfo incorrectInterface;
    incorrectInterface.className = "";  // Пустое имя
    incorrectInterface.namespaceName = "";
    
    // Должен либо бросить исключение, либо обработать корректно
    EXPECT_NO_THROW({
        std::string result = generator->generateAdapter(incorrectInterface);
        // Если не бросает исключение, результат не должен быть пустым
        EXPECT_FALSE(result.empty());
    });
}

/**
 * @brief Тест производительности генерации большого количества методов
 */
TEST_F(AdapterCodeGeneratorTest, PerformanceWithManyMethods) {
    InterfaceInfo largeInterface;
    largeInterface.className = "ILargeInterface";
    largeInterface.namespaceName = "";
    
    // Создаем много методов
    for (int i = 0; i < 100; ++i) {
        MethodInfo method;
        method.name = "method" + std::to_string(i);
        method.returnType = "void";
        method.isPureVirtual = true;
        largeInterface.methods.push_back(method);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string result = generator->generateAdapter(largeInterface);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Генерация не должна занимать слишком много времени (меньше 1 секунды)
    EXPECT_LT(duration.count(), 1000);
    
    // Результат должен содержать все методы
    for (int i = 0; i < 100; ++i) {
        std::string methodName = "method" + std::to_string(i);
        EXPECT_NE(result.find(methodName), std::string::npos);
    }
}