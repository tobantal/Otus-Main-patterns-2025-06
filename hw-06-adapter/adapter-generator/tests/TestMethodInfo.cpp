/**
 * @file TestMethodInfo.cpp
 * @brief Unit-тесты для структуры MethodInfo
 * @author Anton Tobolkin
 * @version 1.0
 */

#include <gtest/gtest.h>
#include "../include/models/MethodInfo.hpp"

/**
 * @brief Тестовый класс для MethodInfo
 */
class MethodInfoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка базового метода для тестов
        baseMethod.name = "testMethod";
        baseMethod.returnType = "void";
        baseMethod.isConst = false;
        baseMethod.isPureVirtual = true;
    }

    MethodInfo baseMethod;
};

/**
 * @brief Тест определения getter методов
 */
TEST_F(MethodInfoTest, IsGetterTest) {
    // Тестируем различные getter методы
    MethodInfo getPosition;
    getPosition.name = "getPosition";
    getPosition.returnType = "Vector";
    getPosition.isConst = true;
    EXPECT_TRUE(getPosition.isGetter());
    
    MethodInfo getValue;
    getValue.name = "getValue";
    getValue.returnType = "int";
    EXPECT_TRUE(getValue.isGetter());
    
    MethodInfo getComplexType;
    getComplexType.name = "getComplexType";
    getComplexType.returnType = "std::shared_ptr<Object>";
    EXPECT_TRUE(getComplexType.isGetter());
    
    // Не getter методы
    MethodInfo setPosition;
    setPosition.name = "setPosition";
    setPosition.returnType = "void";
    EXPECT_FALSE(setPosition.isGetter());
    
    MethodInfo regularMethod;
    regularMethod.name = "calculateSomething";
    regularMethod.returnType = "int";
    EXPECT_FALSE(regularMethod.isGetter());
}

/**
 * @brief Тест определения setter методов
 */
TEST_F(MethodInfoTest, IsSetterTest) {
    // Тестируем различные setter методы
    MethodInfo setPosition;
    setPosition.name = "setPosition";
    setPosition.returnType = "void";
    setPosition.parameters = {"const Vector& pos"};
    EXPECT_TRUE(setPosition.isSetter());
    
    MethodInfo setValue;
    setValue.name = "setValue";
    setValue.returnType = "void";
    setValue.parameters = {"int value"};
    EXPECT_TRUE(setValue.isSetter());
    
    MethodInfo setComplexType;
    setComplexType.name = "setComplexType";
    setComplexType.returnType = "void";
    setComplexType.parameters = {"const std::shared_ptr<Object>& obj"};
    EXPECT_TRUE(setComplexType.isSetter());
    
    // Не setter методы
    MethodInfo getPosition;
    getPosition.name = "getPosition";
    getPosition.returnType = "Vector";
    EXPECT_FALSE(getPosition.isSetter());
    
    MethodInfo setWithoutParams;
    setWithoutParams.name = "setPosition";
    setWithoutParams.returnType = "void";
    // Нет параметров
    EXPECT_FALSE(setWithoutParams.isSetter());
    
    MethodInfo setWithTwoParams;
    setWithTwoParams.name = "setPosition";
    setWithTwoParams.returnType = "void";
    setWithTwoParams.parameters = {"int x", "int y"};
    EXPECT_FALSE(setWithTwoParams.isSetter());
    
    MethodInfo setWithNonVoidReturn;
    setWithNonVoidReturn.name = "setPosition";
    setWithNonVoidReturn.returnType = "bool";
    setWithNonVoidReturn.parameters = {"const Vector& pos"};
    EXPECT_FALSE(setWithNonVoidReturn.isSetter());
}

/**
 * @brief Тест определения void методов
 */
TEST_F(MethodInfoTest, IsVoidMethodTest) {
    // Void методы без параметров
    MethodInfo finish;
    finish.name = "finish";
    finish.returnType = "void";
    EXPECT_TRUE(finish.isVoidMethod());
    
    MethodInfo reset;
    reset.name = "reset";
    reset.returnType = "void";
    EXPECT_TRUE(reset.isVoidMethod());
    
    MethodInfo cleanup;
    cleanup.name = "cleanup";
    cleanup.returnType = "void";
    EXPECT_TRUE(cleanup.isVoidMethod());
    
    // Не void методы
    MethodInfo getPosition;
    getPosition.name = "getPosition";
    getPosition.returnType = "Vector";
    EXPECT_FALSE(getPosition.isVoidMethod());
    
    MethodInfo voidWithParams;
    voidWithParams.name = "setPosition";
    voidWithParams.returnType = "void";
    voidWithParams.parameters = {"const Vector& pos"};
    EXPECT_FALSE(voidWithParams.isVoidMethod());
}

/**
 * @brief Тест извлечения имени свойства из имени метода
 */
TEST_F(MethodInfoTest, GetPropertyNameTest) {
    // Getter методы
    MethodInfo getPosition;
    getPosition.name = "getPosition";
    EXPECT_EQ(getPosition.getPropertyName(), "Position");
    
    MethodInfo getValue;
    getValue.name = "getValue";
    EXPECT_EQ(getValue.getPropertyName(), "Value");
    
    MethodInfo getAngularVelocity;
    getAngularVelocity.name = "getAngularVelocity";  
    EXPECT_EQ(getAngularVelocity.getPropertyName(), "AngularVelocity");
    
    // Setter методы
    MethodInfo setPosition;
    setPosition.name = "setPosition";
    EXPECT_EQ(setPosition.getPropertyName(), "Position");
    
    MethodInfo setValue;
    setValue.name = "setValue";
    EXPECT_EQ(setValue.getPropertyName(), "Value");
    
    // Обычные методы (возвращают исходное имя)
    MethodInfo finish;
    finish.name = "finish";
    EXPECT_EQ(finish.getPropertyName(), "finish");
    
    MethodInfo calculateSomething;
    calculateSomething.name = "calculateSomething";
    EXPECT_EQ(calculateSomething.getPropertyName(), "calculateSomething");
}

/**
 * @brief Тест работы с кастомными реализациями
 */
TEST_F(MethodInfoTest, CustomImplementationTest) {
    MethodInfo method;
    method.name = "getVelocity";
    method.returnType = "Vector";
    
    // Изначально кастомной реализации нет
    EXPECT_FALSE(method.hasCustomImplementation());
    
    // Устанавливаем кастомную реализацию
    method.customImpl.hasCustom = true;
    method.customImpl.className = "MovableImplementations";
    method.customImpl.methodName = "calculateVelocity";
    method.customImpl.fullReference = "MovableImplementations::calculateVelocity";
    
    EXPECT_TRUE(method.hasCustomImplementation());
    EXPECT_EQ(method.customImpl.className, "MovableImplementations");
    EXPECT_EQ(method.customImpl.methodName, "calculateVelocity");
    EXPECT_EQ(method.customImpl.getFullReference(), "MovableImplementations::calculateVelocity");
}

/**
 * @brief Тест комбинированной логики определения типов методов
 */
TEST_F(MethodInfoTest, MethodTypeLogicTest) {
    // Метод может быть только одного типа: getter, setter или void method
    
    // Getter (не setter и не void method)
    MethodInfo getter;
    getter.name = "getPosition";
    getter.returnType = "Vector";
    EXPECT_TRUE(getter.isGetter());
    EXPECT_FALSE(getter.isSetter());
    EXPECT_FALSE(getter.isVoidMethod());
    
    // Setter (не getter и не void method)
    MethodInfo setter;
    setter.name = "setPosition";
    setter.returnType = "void";
    setter.parameters = {"const Vector& pos"};
    EXPECT_FALSE(setter.isGetter());
    EXPECT_TRUE(setter.isSetter());
    EXPECT_FALSE(setter.isVoidMethod());
    
    // Void method (не getter и не setter)
    MethodInfo voidMethod;
    voidMethod.name = "finish";
    voidMethod.returnType = "void";
    EXPECT_FALSE(voidMethod.isGetter());
    EXPECT_FALSE(voidMethod.isSetter());
    EXPECT_TRUE(voidMethod.isVoidMethod());
    
    // Обычный метод (не getter, не setter, не void method)
    MethodInfo regularMethod;
    regularMethod.name = "calculateDistance";
    regularMethod.returnType = "double";
    regularMethod.parameters = {"const Vector& from", "const Vector& to"};
    EXPECT_FALSE(regularMethod.isGetter());
    EXPECT_FALSE(regularMethod.isSetter());
    EXPECT_FALSE(regularMethod.isVoidMethod());
}

/**
 * @brief Тест граничных случаев с именами методов
 */
TEST_F(MethodInfoTest, EdgeCasesTest) {
    // Методы с именами, начинающимися с "get" и "set", но не являющимися геттерами/сеттерами
    
    // "get" в середине имени - не getter
    MethodInfo notGetter;
    notGetter.name = "forgetPosition";
    notGetter.returnType = "void";
    EXPECT_FALSE(notGetter.isGetter());
    
    // "set" в середине имени - не setter
    MethodInfo notSetter;
    notSetter.name = "resetPosition";
    notSetter.returnType = "void";
    notSetter.parameters = {"const Vector& pos"};
    EXPECT_FALSE(notSetter.isSetter());
    
    // Очень короткие имена
    MethodInfo shortGet;
    shortGet.name = "get";
    shortGet.returnType = "int";
    EXPECT_FALSE(shortGet.isGetter()); // Слишком короткое для getter
    
    MethodInfo shortSet;
    shortSet.name = "set";
    shortSet.returnType = "void";
    shortSet.parameters = {"int value"};
    EXPECT_FALSE(shortSet.isSetter()); // Слишком короткое для setter
    
    // Пустое имя
    MethodInfo emptyName;
    emptyName.name = "";
    emptyName.returnType = "void";
    EXPECT_FALSE(emptyName.isGetter());
    EXPECT_FALSE(emptyName.isSetter());
    EXPECT_TRUE(emptyName.isVoidMethod()); // Технически void метод без параметров
}

/**
 * @brief Тест const модификатора
 */
TEST_F(MethodInfoTest, ConstModifierTest) {
    MethodInfo constMethod;
    constMethod.name = "getPosition";
    constMethod.returnType = "Vector";
    constMethod.isConst = true;
    constMethod.isPureVirtual = true;
    
    EXPECT_TRUE(constMethod.isConst);
    EXPECT_TRUE(constMethod.isGetter());
    
    MethodInfo nonConstMethod;
    nonConstMethod.name = "getPosition";
    nonConstMethod.returnType = "Vector";
    nonConstMethod.isConst = false;
    nonConstMethod.isPureVirtual = true;
    
    EXPECT_FALSE(nonConstMethod.isConst);
    EXPECT_TRUE(nonConstMethod.isGetter()); // Все равно getter
}

/**
 * @brief Тест чисто виртуальных методов
 */
TEST_F(MethodInfoTest, PureVirtualTest) {
    MethodInfo pureVirtualMethod;
    pureVirtualMethod.name = "testMethod";
    pureVirtualMethod.returnType = "void";
    pureVirtualMethod.isPureVirtual = true;
    
    EXPECT_TRUE(pureVirtualMethod.isPureVirtual);
    
    MethodInfo regularVirtualMethod;
    regularVirtualMethod.name = "testMethod";
    regularVirtualMethod.returnType = "void";
    regularVirtualMethod.isPureVirtual = false;
    
    EXPECT_FALSE(regularVirtualMethod.isPureVirtual);
}

/**
 * @brief Тест сложных сигнатур методов
 */
TEST_F(MethodInfoTest, ComplexSignaturesTest) {
    MethodInfo complexMethod;
    complexMethod.name = "processObjects";
    complexMethod.returnType = "std::vector<std::shared_ptr<Result>>";
    complexMethod.parameters = {
        "const std::vector<std::shared_ptr<Object>>& objects",
        "std::function<bool(const Object&)> predicate",
        "int flags = 0"
    };
    complexMethod.isConst = true;
    complexMethod.isPureVirtual = true;
    
    EXPECT_FALSE(complexMethod.isGetter()); // Имя не начинается с "get"
    EXPECT_FALSE(complexMethod.isSetter()); // Не void и не начинается с "set"
    EXPECT_FALSE(complexMethod.isVoidMethod()); // Не void возврат
    EXPECT_EQ(complexMethod.parameters.size(), 3);
    EXPECT_TRUE(complexMethod.isConst);
}

/**
 * @brief Тест копирования и присваивания
 */
TEST_F(MethodInfoTest, CopyAssignmentTest) {
    MethodInfo original;
    original.name = "getPosition";
    original.returnType = "Vector";
    original.parameters = {"int x", "int y"};
    original.isConst = true;
    original.isPureVirtual = false;
    original.customImpl.hasCustom = true;
    original.customImpl.className = "TestClass";
    original.customImpl.methodName = "testMethod";
    
    // Тест копирования
    MethodInfo copied = original;
    EXPECT_EQ(copied.name, original.name);
    EXPECT_EQ(copied.returnType, original.returnType);
    EXPECT_EQ(copied.parameters.size(), original.parameters.size());
    EXPECT_EQ(copied.isConst, original.isConst);
    EXPECT_EQ(copied.isPureVirtual, original.isPureVirtual);
    EXPECT_EQ(copied.customImpl.hasCustom, original.customImpl.hasCustom);
    EXPECT_EQ(copied.customImpl.className, original.customImpl.className);
    
    // Тест присваивания
    MethodInfo assigned;
    assigned = original;
    EXPECT_EQ(assigned.name, original.name);
    EXPECT_EQ(assigned.returnType, original.returnType);
    EXPECT_EQ(assigned.parameters.size(), original.parameters.size());
}