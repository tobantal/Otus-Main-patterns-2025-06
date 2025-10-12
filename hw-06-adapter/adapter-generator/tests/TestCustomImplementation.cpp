/**
 * @file TestCustomImplementation.cpp
 * @brief Unit-тесты для структуры CustomImplementation
 * @author Anton Tobolkin
 * @version 1.0
 */

#include <gtest/gtest.h>
#include "../include/models/CustomImplementation.hpp"

/**
 * @brief Тестовый класс для CustomImplementation
 */
class CustomImplementationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Базовая настройка
        validImpl.hasCustom = true;
        validImpl.className = "TestClass";
        validImpl.methodName = "testMethod";
        validImpl.fullReference = "TestClass::testMethod";
    }

    CustomImplementation validImpl;
};

/**
 * @brief Тест валидации корректной кастомной реализации
 */
TEST_F(CustomImplementationTest, ValidImplementationTest) {
    EXPECT_TRUE(validImpl.isValid());
    EXPECT_TRUE(validImpl.hasCustom);
    EXPECT_EQ(validImpl.className, "TestClass");
    EXPECT_EQ(validImpl.methodName, "testMethod");
    EXPECT_EQ(validImpl.getFullReference(), "TestClass::testMethod");
}

/**
 * @brief Тест невалидных кастомных реализаций
 */
TEST_F(CustomImplementationTest, InvalidImplementationTest) {
    // Тест с hasCustom = false
    CustomImplementation impl1;
    impl1.hasCustom = false;
    impl1.className = "TestClass";
    impl1.methodName = "testMethod";
    EXPECT_FALSE(impl1.isValid());

    // Тест с пустым именем класса
    CustomImplementation impl2;
    impl2.hasCustom = true;
    impl2.className = "";
    impl2.methodName = "testMethod";
    EXPECT_FALSE(impl2.isValid());

    // Тест с пустым именем метода
    CustomImplementation impl3;
    impl3.hasCustom = true;
    impl3.className = "TestClass";
    impl3.methodName = "";
    EXPECT_FALSE(impl3.isValid());

    // Тест с пустыми именами класса и метода
    CustomImplementation impl4;
    impl4.hasCustom = true;
    impl4.className = "";
    impl4.methodName = "";
    EXPECT_FALSE(impl4.isValid());
}

/**
 * @brief Тест парсинга корректных аннотаций
 */
TEST_F(CustomImplementationTest, ParseValidAnnotationsTest) {
    CustomImplementation impl;
    
    // Простая аннотация
    EXPECT_TRUE(impl.parseFromAnnotation("TestClass::testMethod"));
    EXPECT_TRUE(impl.hasCustom);
    EXPECT_EQ(impl.className, "TestClass");
    EXPECT_EQ(impl.methodName, "testMethod");
    EXPECT_EQ(impl.getFullReference(), "TestClass::testMethod");

    // Аннотация с namespace
    CustomImplementation impl2;
    EXPECT_TRUE(impl2.parseFromAnnotation("MyNamespace::TestClass::testMethod"));
    EXPECT_TRUE(impl2.hasCustom);
    EXPECT_EQ(impl2.className, "MyNamespace::TestClass");
    EXPECT_EQ(impl2.methodName, "testMethod");
    EXPECT_EQ(impl2.getFullReference(), "MyNamespace::TestClass::testMethod");

    // Аннотация с вложенными namespace
    CustomImplementation impl3;
    EXPECT_TRUE(impl3.parseFromAnnotation("Outer::Inner::TestClass::testMethod"));
    EXPECT_TRUE(impl3.hasCustom);
    EXPECT_EQ(impl3.className, "Outer::Inner::TestClass");
    EXPECT_EQ(impl3.methodName, "testMethod");
    EXPECT_EQ(impl3.getFullReference(), "Outer::Inner::TestClass::testMethod");
}

/**
 * @brief Тест парсинга некорректных аннотаций
 */
TEST_F(CustomImplementationTest, ParseInvalidAnnotationsTest) {
    CustomImplementation impl;

    // Пустая аннотация
    EXPECT_FALSE(impl.parseFromAnnotation(""));
    EXPECT_FALSE(impl.hasCustom);

    // Аннотация без разделителя
    EXPECT_FALSE(impl.parseFromAnnotation("TestClasstestMethod"));
    EXPECT_FALSE(impl.hasCustom);

    // Аннотация только с именем класса
    EXPECT_FALSE(impl.parseFromAnnotation("TestClass::"));
    EXPECT_FALSE(impl.hasCustom);

    // Аннотация только с именем метода
    EXPECT_FALSE(impl.parseFromAnnotation("::testMethod"));
    EXPECT_FALSE(impl.hasCustom);

    // Аннотация с только разделителем
    EXPECT_FALSE(impl.parseFromAnnotation("::"));
    EXPECT_FALSE(impl.hasCustom);

    // Аннотация с пробелами (должна обрабатываться)
    CustomImplementation impl2;
    EXPECT_TRUE(impl2.parseFromAnnotation(" TestClass::testMethod "));
    EXPECT_TRUE(impl2.hasCustom);
    EXPECT_EQ(impl2.className, "TestClass");
    EXPECT_EQ(impl2.methodName, "testMethod");
}

/**
 * @brief Тест получения полной ссылки
 */
TEST_F(CustomImplementationTest, GetFullReferenceTest) {
    EXPECT_EQ(validImpl.getFullReference(), "TestClass::testMethod");

    // Тест с пустыми полями
    CustomImplementation emptyImpl;
    EXPECT_EQ(emptyImpl.getFullReference(), "");

    // Тест с только именем класса
    CustomImplementation classOnlyImpl;
    classOnlyImpl.className = "TestClass";
    EXPECT_EQ(classOnlyImpl.getFullReference(), "");

    // Тест с только именем метода
    CustomImplementation methodOnlyImpl;
    methodOnlyImpl.methodName = "testMethod";
    EXPECT_EQ(methodOnlyImpl.getFullReference(), "");
}

/**
 * @brief Тест копирования и присваивания
 */
TEST_F(CustomImplementationTest, CopyAssignmentTest) {
    // Тест копирования
    CustomImplementation copied = validImpl;
    EXPECT_EQ(copied.hasCustom, validImpl.hasCustom);
    EXPECT_EQ(copied.className, validImpl.className);
    EXPECT_EQ(copied.methodName, validImpl.methodName);
    EXPECT_EQ(copied.getFullReference(), validImpl.getFullReference());
    EXPECT_TRUE(copied.isValid());

    // Тест присваивания
    CustomImplementation assigned;
    assigned = validImpl;
    EXPECT_EQ(assigned.hasCustom, validImpl.hasCustom);
    EXPECT_EQ(assigned.className, validImpl.className);
    EXPECT_EQ(assigned.methodName, validImpl.methodName);
    EXPECT_EQ(assigned.getFullReference(), validImpl.getFullReference());
    EXPECT_TRUE(assigned.isValid());
}

/**
 * @brief Тест изменения полей после парсинга
 */
TEST_F(CustomImplementationTest, ModificationAfterParsingTest) {
    CustomImplementation impl;
    EXPECT_TRUE(impl.parseFromAnnotation("OriginalClass::originalMethod"));
    
    // Изменяем поля
    impl.className = "ModifiedClass";
    impl.methodName = "modifiedMethod";
    
    // fullReference не обновляется автоматически при изменении полей
    EXPECT_EQ(impl.fullReference, "OriginalClass::originalMethod");
    
    // Но getFullReference() возвращает актуальное значение
    EXPECT_EQ(impl.getFullReference(), "ModifiedClass::modifiedMethod");
}

/**
 * @brief Тест граничных случаев
 */
TEST_F(CustomImplementationTest, EdgeCasesTest) {
    CustomImplementation impl;

    // Аннотация с множественными разделителями
    EXPECT_FALSE(impl.parseFromAnnotation("Test::Class::::method"));
    
    // Аннотация с очень длинными именами
    std::string longClassName(1000, 'A');
    std::string longMethodName(1000, 'B');
    std::string longAnnotation = longClassName + "::" + longMethodName;
    
    CustomImplementation longImpl;
    EXPECT_TRUE(longImpl.parseFromAnnotation(longAnnotation));
    EXPECT_EQ(longImpl.className, longClassName);
    EXPECT_EQ(longImpl.methodName, longMethodName);

    // Аннотация с специальными символами в именах
    CustomImplementation specialImpl;
    EXPECT_TRUE(specialImpl.parseFromAnnotation("Test_Class123::test_method_456"));
    EXPECT_EQ(specialImpl.className, "Test_Class123");
    EXPECT_EQ(specialImpl.methodName, "test_method_456");
}

/**
 * @brief Тест состояния по умолчанию
 */
TEST_F(CustomImplementationTest, DefaultStateTest) {
    CustomImplementation defaultImpl;
    
    EXPECT_FALSE(defaultImpl.hasCustom);
    EXPECT_TRUE(defaultImpl.className.empty());
    EXPECT_TRUE(defaultImpl.methodName.empty());
    EXPECT_TRUE(defaultImpl.fullReference.empty());
    EXPECT_FALSE(defaultImpl.isValid());
    EXPECT_EQ(defaultImpl.getFullReference(), "");
}

/**
 * @brief Тест обновления fullReference при парсинге
 */
TEST_F(CustomImplementationTest, FullReferenceUpdateTest) {
    CustomImplementation impl;
    
    // Изначально fullReference пуста
    EXPECT_TRUE(impl.getFullReference().empty());
    
    // После парсинга fullReference должна обновиться
    EXPECT_TRUE(impl.parseFromAnnotation("TestClass::testMethod"));
    EXPECT_EQ(impl.getFullReference(), "TestClass::testMethod");
    
    // При повторном парсинге fullReference должна обновиться
    EXPECT_TRUE(impl.parseFromAnnotation("AnotherClass::anotherMethod"));
    EXPECT_EQ(impl.getFullReference(), "AnotherClass::anotherMethod");
}

/**
 * @brief Тест производительности парсинга
 */
TEST_F(CustomImplementationTest, ParsingPerformanceTest) {
    const int iterationCount = 10000;
    std::vector<std::string> annotations = {
        "Class1::method1",
        "Namespace::Class2::method2",
        "Outer::Inner::Class3::method3",
        "VeryLongNamespace::VeryLongClassName::veryLongMethodName"
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterationCount; ++i) {
        CustomImplementation impl;
        const std::string& annotation = annotations[i % annotations.size()];
        bool result = impl.parseFromAnnotation(annotation);
        EXPECT_TRUE(result); // Убеждаемся, что парсинг успешен
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Performance test: parsed " << iterationCount 
              << " annotations in " << duration.count() << "ms" << std::endl;
    
    // Парсинг должен быть быстрым (менее 1 секунды для 10000 итераций)
    EXPECT_LT(duration.count(), 1000);
}