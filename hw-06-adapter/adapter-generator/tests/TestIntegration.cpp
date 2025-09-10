/**
 * @file TestIntegration.cpp
 * @brief Интеграционные тесты для генератора адаптеров
 * @author Anton Tobolkin  
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../include/parsers/SimpleInterfaceParser.hpp"
#include "../include/generators/AdapterCodeGenerator.hpp"
#include "../include/utils/FileUtils.hpp"

/**
 * @brief Интеграционные тесты полного pipeline генерации адаптеров
 */
class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser = std::make_unique<SimpleInterfaceParser>();
        generator = std::make_unique<AdapterCodeGenerator>();
        
        // Создаем временную директорию для тестов
        testDir = std::filesystem::temp_directory_path() / "adapter_generator_test";
        std::filesystem::create_directories(testDir);
        
        setupTestFiles();
    }

    void TearDown() override {
        // Очищаем временные файлы
        std::filesystem::remove_all(testDir);
        
        parser.reset();
        generator.reset();
    }

    void setupTestFiles() {
        // Создаем тестовый интерфейс IMovable
        std::string movableInterface = R"(
#pragma once

class IMovable {
public:
    virtual ~IMovable() = default;
    virtual Vector getPosition() const = 0;
    virtual void setPosition(const Vector& newPosition) = 0;
    virtual Vector getVelocity() const = 0;
};
        )";
        
        FileUtils::writeFile(testDir / "IMovable.hpp", movableInterface);
        
        // Создаем тестовый интерфейс IRotatable
        std::string rotatableInterface = R"(
#pragma once

namespace SpaceShip::Operations {
    class IRotatable {
    public:
        virtual ~IRotatable() = default;
        virtual int getDirection() const = 0;
        virtual void setDirection(int direction) = 0;
        virtual int getAngularVelocity() const = 0;
        virtual int getDirectionsNumber() const = 0;
    };
}
        )";
        
        FileUtils::writeFile(testDir / "IRotatable.hpp", rotatableInterface);
        
        // Создаем интерфейс с void методами
        std::string finishableInterface = R"(
#pragma once

class IFinishable {
public:
    virtual ~IFinishable() = default;
    
    /**
     * @brief Финализирует операцию
     */
    virtual void finish() = 0;
    
    /**
     * @brief Сбрасывает состояние
     */
    virtual void reset() = 0;
    
    /**
     * @brief Очищает ресурсы
     */
    virtual void cleanup() = 0;
};
        )";
        
        FileUtils::writeFile(testDir / "IFinishable.hpp", finishableInterface);
        
        // Создаем интерфейс с кастомными реализациями
        std::string complexInterface = R"(
#pragma once

class IComplexMovable {
public:
    virtual ~IComplexMovable() = default;
    virtual Vector getPosition() const = 0;
    virtual void setPosition(const Vector& newPosition) = 0;
    
    /**
     * @brief Получает скорость с учетом физики
     * @custom_impl MovableImplementations::calculatePhysicsVelocity
     */
    virtual Vector getVelocity() const = 0;
    
    /**
     * @brief Сложный расчет ускорения
     * @custom_impl MovableImplementations::calculateAcceleration
     */
    virtual Vector getAcceleration() const = 0;
};
        )";
        
        FileUtils::writeFile(testDir / "IComplexMovable.hpp", complexInterface);
    }

    std::unique_ptr<SimpleInterfaceParser> parser;
    std::unique_ptr<AdapterCodeGenerator> generator;
    std::filesystem::path testDir;
};

/**
 * @brief Тест полного pipeline: файл -> парсинг -> генерация -> сохранение
 */
TEST_F(IntegrationTest, FullPipelineTest) {
    // 1. Находим все интерфейсные файлы
    auto interfaceFiles = FileUtils::findFilesWithExtension(testDir.string(), ".hpp");
    EXPECT_EQ(interfaceFiles.size(), 4);
    
    // 2. Парсим все файлы
    std::vector<InterfaceInfo> allInterfaces;
    for (const auto& file : interfaceFiles) {
        auto interfaces = parser->parseFile(file);
        allInterfaces.insert(allInterfaces.end(), interfaces.begin(), interfaces.end());
    }
    
    // Проверяем, что найдены все интерфейсы
    EXPECT_EQ(allInterfaces.size(), 4);
    
    // Находим конкретные интерфейсы
    auto movable = std::find_if(allInterfaces.begin(), allInterfaces.end(),
        [](const InterfaceInfo& info) { return info.className == "IMovable"; });
    auto rotatable = std::find_if(allInterfaces.begin(), allInterfaces.end(),
        [](const InterfaceInfo& info) { return info.className == "IRotatable"; });
    auto finishable = std::find_if(allInterfaces.begin(), allInterfaces.end(),
        [](const InterfaceInfo& info) { return info.className == "IFinishable"; });
    auto complex = std::find_if(allInterfaces.begin(), allInterfaces.end(),
        [](const InterfaceInfo& info) { return info.className == "IComplexMovable"; });
    
    ASSERT_NE(movable, allInterfaces.end());
    ASSERT_NE(rotatable, allInterfaces.end());
    ASSERT_NE(finishable, allInterfaces.end());
    ASSERT_NE(complex, allInterfaces.end());
    
    // Проверяем правильность парсинга
    EXPECT_EQ(movable->methods.size(), 3);
    EXPECT_EQ(rotatable->methods.size(), 4);
    EXPECT_EQ(rotatable->namespaceName, "SpaceShip::Operations");
    EXPECT_EQ(finishable->methods.size(), 3);
    EXPECT_EQ(complex->methods.size(), 4);
    
    // 3. Генерируем код адаптеров
    std::string outputDir = (testDir / "generated").string();
    FileUtils::createDirectories(outputDir);
    
    // Генерируем индивидуальные адаптеры
    for (const auto& interface : allInterfaces) {
        std::string adapterCode = generator->generateAdapter(interface);
        EXPECT_FALSE(adapterCode.empty());
        
        std::string filename = interface.getAdapterName() + ".hpp";
        FileUtils::writeFile(outputDir + "/" + filename, adapterCode);
        
        // Проверяем, что файл создан
        EXPECT_TRUE(FileUtils::fileExists(outputDir + "/" + filename));
    }
    
    // 4. Генерируем код регистрации
    std::string registrationCode = generator->generateRegistrationCode(allInterfaces);
    FileUtils::writeFile(outputDir + "/AdapterRegistry.hpp", registrationCode);
    
    EXPECT_TRUE(FileUtils::fileExists(outputDir + "/AdapterRegistry.hpp"));
    
    // 5. Проверяем содержимое сгенерированных файлов
    std::string movableAdapter = FileUtils::readFile(outputDir + "/MovableAdapter.hpp");
    EXPECT_NE(movableAdapter.find("class AutoGenerated::MovableAdapter"), std::string::npos);
    EXPECT_NE(movableAdapter.find("public IMovable"), std::string::npos);
    
    std::string rotatableAdapter = FileUtils::readFile(outputDir + "/RotatableAdapter.hpp");
    EXPECT_NE(rotatableAdapter.find("public SpaceShip::Operations::IRotatable"), std::string::npos);
    
    std::string registryContent = FileUtils::readFile(outputDir + "/AdapterRegistry.hpp");
    EXPECT_NE(registryContent.find("class AdapterRegistration"), std::string::npos);
    EXPECT_NE(registryContent.find("\"Adapter:IMovable\""), std::string::npos);
    EXPECT_NE(registryContent.find("\"Adapter:SpaceShip::Operations::IRotatable\""), std::string::npos);
}

/**
 * @brief Тест генерации адаптеров с различными типами методов
 */
TEST_F(IntegrationTest, MethodTypeHandlingTest) {
    // Парсим интерфейс IComplexMovable
    auto interfaces = parser->parseFile((testDir / "IComplexMovable.hpp").string());
    ASSERT_EQ(interfaces.size(), 1);
    
    const auto& interface = interfaces[0];
    ASSERT_EQ(interface.methods.size(), 4);
    
    // Проверяем типы методов
    bool hasGetter = false, hasSetter = false, hasCustom = false;
    
    for (const auto& method : interface.methods) {
        if (method.isGetter()) hasGetter = true;
        if (method.isSetter()) hasSetter = true;
        if (method.hasCustomImplementation()) hasCustom = true;
    }
    
    EXPECT_TRUE(hasGetter);
    EXPECT_TRUE(hasSetter);
    EXPECT_TRUE(hasCustom);
    
    // Генерируем код и проверяем обработку различных типов
    std::string adapterCode = generator->generateAdapter(interface);
    
    // Getter методы должны использовать IoC::resolve<ReturnType>
    EXPECT_NE(adapterCode.find("IoC::resolve<Vector>(\"IComplexMovable:Position.get\""), std::string::npos);
    
    // Setter методы должны вызывать execute()
    EXPECT_NE(adapterCode.find("IoC::resolve(\"IComplexMovable:Position.set\""), std::string::npos);
    EXPECT_NE(adapterCode.find("->execute()"), std::string::npos);
    
    // Кастомные методы должны использовать статические вызовы
    EXPECT_NE(adapterCode.find("MovableImplementations::calculatePhysicsVelocity"), std::string::npos);
    EXPECT_NE(adapterCode.find("MovableImplementations::calculateAcceleration"), std::string::npos);
}

/**
 * @brief Тест обработки void методов
 */
TEST_F(IntegrationTest, VoidMethodsTest) {
    // Парсим интерфейс IFinishable
    auto interfaces = parser->parseFile((testDir / "IFinishable.hpp").string());
    ASSERT_EQ(interfaces.size(), 1);
    
    const auto& interface = interfaces[0];
    ASSERT_EQ(interface.methods.size(), 3);
    
    // Все методы должны быть void методами
    for (const auto& method : interface.methods) {
        EXPECT_TRUE(method.isVoidMethod());
        EXPECT_EQ(method.returnType, "void");
        EXPECT_TRUE(method.parameters.empty());
    }
    
    // Генерируем код и проверяем обработку void методов
    std::string adapterCode = generator->generateAdapter(interface);
    
    // Void методы должны вызывать IoC::resolve(...)->execute()
    EXPECT_NE(adapterCode.find("IoC::resolve(\"IFinishable:finish\", obj_)->execute()"), std::string::npos);
    EXPECT_NE(adapterCode.find("IoC::resolve(\"IFinishable:reset\", obj_)->execute()"), std::string::npos);
    EXPECT_NE(adapterCode.find("IoC::resolve(\"IFinishable:cleanup\", obj_)->execute()"), std::string::npos);
    
    // Методы должны иметь корректные сигнатуры
    EXPECT_NE(adapterCode.find("void finish() override"), std::string::npos);
    EXPECT_NE(adapterCode.find("void reset() override"), std::string::npos);
    EXPECT_NE(adapterCode.find("void cleanup() override"), std::string::npos);
}

/**
 * @brief Тест стратегии регистрации в IoC контейнере
 */
TEST_F(IntegrationTest, IoCRegistrationStrategyTest) {
    // Парсим все интерфейсы
    auto interfaceFiles = FileUtils::findFilesWithExtension(testDir.string(), ".hpp");
    std::vector<InterfaceInfo> allInterfaces;
    
    for (const auto& file : interfaceFiles) {
        auto interfaces = parser->parseFile(file);
        allInterfaces.insert(allInterfaces.end(), interfaces.begin(), interfaces.end());
    }
    
    // Генерируем код регистрации
    std::string registrationCode = generator->generateRegistrationCode(allInterfaces);
    
    // Проверяем стратегию регистрации:
    
    // 1. Должна быть основная функция регистрации
    EXPECT_NE(registrationCode.find("class AdapterRegistration"), std::string::npos);
    EXPECT_NE(registrationCode.find("static void registerAll()"), std::string::npos);
    
    // 2. Каждый адаптер должен быть зарегистрирован с уникальным ключом
    EXPECT_NE(registrationCode.find("\"Adapter:IMovable\""), std::string::npos);
    EXPECT_NE(registrationCode.find("\"Adapter:SpaceShip::Operations::IRotatable\""), std::string::npos);
    EXPECT_NE(registrationCode.find("\"Adapter:IFinishable\""), std::string::npos);
    EXPECT_NE(registrationCode.find("\"Adapter:IComplexMovable\""), std::string::npos);
    
    // 3. Должны быть фабричные методы
    EXPECT_NE(registrationCode.find("std::make_shared<MovableAdapter>"), std::string::npos);
    EXPECT_NE(registrationCode.find("std::make_shared<RotatableAdapter>"), std::string::npos);
    EXPECT_NE(registrationCode.find("std::make_shared<FinishableAdapter>"), std::string::npos);
    EXPECT_NE(registrationCode.find("std::make_shared<ComplexMovableAdapter>"), std::string::npos);
    
    // 4. Все регистрации должны использовать IoC::resolve("IoC.Register", ...)
    size_t registerCount = 0;
    size_t pos = 0;
    while ((pos = registrationCode.find("IoC::resolve(\"IoC.Register\"", pos)) != std::string::npos) {
        registerCount++;
        pos++;
    }
    EXPECT_EQ(registerCount, 4); // По одной регистрации для каждого интерфейса
}

/**
 * @brief Тест обработки ошибок и граничных случаев
 */
TEST_F(IntegrationTest, ErrorHandlingTest) {
    // Тест с несуществующим файлом
    EXPECT_THROW(parser->parseFile("non_existent_file.hpp"), std::runtime_error);
    
    // Тест с пустым файлом
    std::string emptyFile = (testDir / "empty.hpp").string();
    FileUtils::writeFile(emptyFile, "");
    
    auto interfaces = parser->parseFile(emptyFile);
    EXPECT_TRUE(interfaces.empty());
    
    // Тест с файлом без интерфейсов
    std::string noInterfaceFile = (testDir / "no_interface.hpp").string();
    FileUtils::writeFile(noInterfaceFile, R"(
        class RegularClass {
        public:
            void method() {}
        };
    )");
    
    interfaces = parser->parseFile(noInterfaceFile);
    EXPECT_TRUE(interfaces.empty());
}

/**
 * @brief Тест производительности обработки большого количества файлов
 */
TEST_F(IntegrationTest, PerformanceTest) {
    // Создаем множество интерфейсов
    const int interfaceCount = 50;
    std::vector<std::string> createdFiles;
    
    for (int i = 0; i < interfaceCount; ++i) {
        std::string interfaceContent = R"(
#pragma once

class ITestInterface)" + std::to_string(i) + R"( {
public:
    virtual ~ITestInterface)" + std::to_string(i) + R"(() = default;
    virtual int getValue)" + std::to_string(i) + R"(() const = 0;
    virtual void setValue)" + std::to_string(i) + R"((int value) = 0;
    virtual void process)" + std::to_string(i) + R"(() = 0;
};
        )";
        
        std::string filename = "ITestInterface" + std::to_string(i) + ".hpp";
        std::string filepath = (testDir / filename).string();
        FileUtils::writeFile(filepath, interfaceContent);
        createdFiles.push_back(filepath);
    }
    
    // Измеряем время парсинга
    auto startParsing = std::chrono::high_resolution_clock::now();
    
    std::vector<InterfaceInfo> allInterfaces;
    for (const auto& file : createdFiles) {
        auto interfaces = parser->parseFile(file);
        allInterfaces.insert(allInterfaces.end(), interfaces.begin(), interfaces.end());
    }
    
    auto endParsing = std::chrono::high_resolution_clock::now();
    auto parsingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endParsing - startParsing);
    
    EXPECT_EQ(allInterfaces.size(), interfaceCount);
    
    // Измеряем время генерации
    auto startGeneration = std::chrono::high_resolution_clock::now();
    
    std::string registrationCode = generator->generateRegistrationCode(allInterfaces);
    
    for (const auto& interface : allInterfaces) {
        std::string adapterCode = generator->generateAdapter(interface);
        EXPECT_FALSE(adapterCode.empty());
    }
    
    auto endGeneration = std::chrono::high_resolution_clock::now();
    auto generationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endGeneration - startGeneration);
    
    // Проверяем производительность (не более 5 секунд на 50 интерфейсов)
    EXPECT_LT(parsingDuration.count(), 5000);
    EXPECT_LT(generationDuration.count(), 5000);
    
    std::cout << "Performance test results:" << std::endl;
    std::cout << "  Parsing " << interfaceCount << " interfaces: " << parsingDuration.count() << "ms" << std::endl;
    std::cout << "  Generating " << interfaceCount << " adapters: " << generationDuration.count() << "ms" << std::endl;
}