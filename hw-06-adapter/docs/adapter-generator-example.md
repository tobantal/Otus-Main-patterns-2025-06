# Пример реализации генератора адаптеров (C++17)

## Простая реализация парсера интерфейсов

```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <map>

struct MethodInfo {
    std::string returnType;
    std::string name;
    std::vector<std::string> parameters;
    bool isConst = false;
    bool isPureVirtual = false;
};

struct InterfaceInfo {
    std::string namespaceName;
    std::string className; 
    std::vector<MethodInfo> methods;
};

class SimpleInterfaceParser {
private:
    // Регулярные выражения для парсинга
    std::regex classRegex_{R"(class\s+(\w+)\s*\{)"};
    std::regex methodRegex_{R"(virtual\s+(.+?)\s+(\w+)\s*\(([^)]*)\)\s*(const)?\s*=\s*0)"};
    std::regex namespaceRegex_{R"(namespace\s+(\w+))"};
    
public:
    std::vector<InterfaceInfo> parseFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        
        return parseContent(content);
    }
    
private:
    std::vector<InterfaceInfo> parseContent(const std::string& content) {
        std::vector<InterfaceInfo> interfaces;
        
        // Поиск классов с чисто виртуальными методами
        std::sregex_iterator classBegin(content.begin(), content.end(), classRegex_);
        std::sregex_iterator classEnd;
        
        for (auto it = classBegin; it != classEnd; ++it) {
            std::smatch match = *it;
            std::string className = match[1].str();
            
            // Поиск методов в классе
            size_t classPos = match.position();
            size_t braceCount = 0;
            size_t classStart = content.find('{', classPos);
            size_t classEnd = findMatchingBrace(content, classStart);
            
            std::string classContent = content.substr(classStart, classEnd - classStart);
            
            auto methods = parseMethods(classContent);
            if (!methods.empty() && hasOnlyPureVirtual(methods)) {
                InterfaceInfo interface;
                interface.className = className;
                interface.namespaceName = extractNamespace(content, classPos);
                interface.methods = std::move(methods);
                interfaces.push_back(interface);
            }
        }
        
        return interfaces;
    }
    
    std::vector<MethodInfo> parseMethods(const std::string& classContent) {
        std::vector<MethodInfo> methods;
        
        std::sregex_iterator methodBegin(classContent.begin(), classContent.end(), methodRegex_);
        std::sregex_iterator methodEnd;
        
        for (auto it = methodBegin; it != methodEnd; ++it) {
            std::smatch match = *it;
            
            MethodInfo method;
            method.returnType = trim(match[1].str());
            method.name = match[2].str();
            method.parameters = parseParameters(match[3].str());
            method.isConst = !match[4].str().empty();
            method.isPureVirtual = true;
            
            methods.push_back(method);
        }
        
        return methods;
    }
    
    std::vector<std::string> parseParameters(const std::string& paramStr) {
        std::vector<std::string> params;
        if (paramStr.empty()) return params;
        
        std::istringstream iss(paramStr);
        std::string param;
        while (std::getline(iss, param, ',')) {
            params.push_back(trim(param));
        }
        return params;
    }
    
    bool hasOnlyPureVirtual(const std::vector<MethodInfo>& methods) {
        return !methods.empty() && std::all_of(methods.begin(), methods.end(),
            [](const MethodInfo& m) { return m.isPureVirtual; });
    }
    
    std::string extractNamespace(const std::string& content, size_t pos) {
        // Поиск namespace перед позицией класса
        auto beforeClass = content.substr(0, pos);
        std::sregex_iterator nsBegin(beforeClass.begin(), beforeClass.end(), namespaceRegex_);
        std::sregex_iterator nsEnd;
        
        std::string result;
        for (auto it = nsBegin; it != nsEnd; ++it) {
            result = it->str(1);
        }
        return result;
    }
    
    size_t findMatchingBrace(const std::string& content, size_t start) {
        int braceCount = 1;
        for (size_t i = start + 1; i < content.size(); ++i) {
            if (content[i] == '{') braceCount++;
            else if (content[i] == '}') {
                braceCount--;
                if (braceCount == 0) return i;
            }
        }
        return content.size();
    }
    
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};
```

## Генератор кода адаптеров

```cpp
class AdapterCodeGenerator {
private:
    static const std::string ADAPTER_TEMPLATE;
    static const std::string GETTER_METHOD_TEMPLATE;
    static const std::string SETTER_METHOD_TEMPLATE;
    static const std::string VOID_METHOD_TEMPLATE;
    
    std::map<std::string, std::string> variables_;
    
public:
    std::string generateAdapter(const InterfaceInfo& interface) {
        variables_.clear();
        
        // Установка основных переменных
        variables_["INTERFACE_NAME"] = interface.className;
        variables_["FULL_INTERFACE_NAME"] = buildFullName(interface);
        variables_["ADAPTER_NAME"] = interface.className.substr(1) + "Adapter"; // Убираем 'I'
        
        // Генерация методов
        std::string methods;
        for (const auto& method : interface.methods) {
            methods += generateMethod(interface, method);
        }
        variables_["METHODS"] = methods;
        
        return renderTemplate(ADAPTER_TEMPLATE, variables_);
    }
    
private:
    std::string buildFullName(const InterfaceInfo& interface) {
        if (interface.namespaceName.empty()) {
            return interface.className;
        }
        return interface.namespaceName + "::" + interface.className;
    }
    
    std::string generateMethod(const InterfaceInfo& interface, const MethodInfo& method) {
        if (method.returnType == "void" && method.parameters.empty()) {
            // Метод типа void finish()
            return generateVoidMethod(interface, method);
        } else if (method.name.substr(0, 3) == "set" && method.parameters.size() == 1) {
            // Setter метод
            return generateSetter(interface, method);
        } else {
            // Getter метод
            return generateGetter(interface, method);
        }
    }
    
    std::string generateGetter(const InterfaceInfo& interface, const MethodInfo& method) {
        std::map<std::string, std::string> vars = variables_;
        vars["RETURN_TYPE"] = method.returnType;
        vars["METHOD_NAME"] = method.name;
        vars["PROPERTY_NAME"] = extractPropertyName(method.name);
        vars["CONST_QUALIFIER"] = method.isConst ? "const " : "";
        
        return renderTemplate(GETTER_METHOD_TEMPLATE, vars);
    }
    
    std::string generateSetter(const InterfaceInfo& interface, const MethodInfo& method) {
        std::map<std::string, std::string> vars = variables_;
        vars["METHOD_NAME"] = method.name;
        vars["PARAM_TYPE"] = extractParamType(method.parameters[0]);
        vars["PROPERTY_NAME"] = extractPropertyName(method.name);
        
        return renderTemplate(SETTER_METHOD_TEMPLATE, vars);
    }
    
    std::string generateVoidMethod(const InterfaceInfo& interface, const MethodInfo& method) {
        std::map<std::string, std::string> vars = variables_;
        vars["METHOD_NAME"] = method.name;
        vars["METHOD_KEY"] = method.name;
        
        return renderTemplate(VOID_METHOD_TEMPLATE, vars);
    }
    
    std::string extractPropertyName(const std::string& methodName) {
        if (methodName.substr(0, 3) == "get" || methodName.substr(0, 3) == "set") {
            return methodName.substr(3);
        }
        return methodName;
    }
    
    std::string extractParamType(const std::string& param) {
        // Извлекаем тип из строки параметра
        auto pos = param.find_last_of(' ');
        if (pos != std::string::npos) {
            return param.substr(0, pos);
        }
        return param;
    }
    
    std::string renderTemplate(const std::string& templateStr, 
                              const std::map<std::string, std::string>& vars) {
        std::string result = templateStr;
        for (const auto& [key, value] : vars) {
            std::string placeholder = "{" + key + "}";
            size_t pos = 0;
            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        }
        return result;
    }
};

// Шаблоны кода
const std::string AdapterCodeGenerator::ADAPTER_TEMPLATE = R"(
class AutoGenerated::{ADAPTER_NAME} : public {FULL_INTERFACE_NAME} {
private:
    UObject* obj_;
    
public:
    explicit {ADAPTER_NAME}(UObject* obj) : obj_(obj) {
        if (!obj) {
            throw std::invalid_argument("Object cannot be null");
        }
    }
{METHODS}
};
)";

const std::string AdapterCodeGenerator::GETTER_METHOD_TEMPLATE = R"(
    {RETURN_TYPE} {METHOD_NAME}() {CONST_QUALIFIER}override {
        return IoC::resolve<{RETURN_TYPE}>("{INTERFACE_NAME}:{PROPERTY_NAME}.get", obj_);
    }
)";

const std::string AdapterCodeGenerator::SETTER_METHOD_TEMPLATE = R"(
    void {METHOD_NAME}(const {PARAM_TYPE}& value) override {
        IoC::resolve<ICommand>("{INTERFACE_NAME}:{PROPERTY_NAME}.set", obj_, value)->execute();
    }
)";

const std::string AdapterCodeGenerator::VOID_METHOD_TEMPLATE = R"(
    void {METHOD_NAME}() override {
        IoC::resolve<ICommand>("{INTERFACE_NAME}:{METHOD_KEY}", obj_)->execute();
    }
)";
```

## Основное приложение генератора

```cpp
#include <iostream>
#include <filesystem>

class AdapterGenerator {
private:
    SimpleInterfaceParser parser_;
    AdapterCodeGenerator generator_;
    
public:
    void generateAdapters(const std::string& inputDir, const std::string& outputFile) {
        std::vector<InterfaceInfo> allInterfaces;
        
        // Поиск всех заголовочных файлов
        for (const auto& entry : std::filesystem::recursive_directory_iterator(inputDir)) {
            if (entry.path().extension() == ".hpp" || entry.path().extension() == ".h") {
                try {
                    auto interfaces = parser_.parseFile(entry.path().string());
                    allInterfaces.insert(allInterfaces.end(), interfaces.begin(), interfaces.end());
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Failed to parse " << entry.path() 
                              << ": " << e.what() << std::endl;
                }
            }
        }
        
        // Генерация кода адаптеров
        generateOutputFile(allInterfaces, outputFile);
    }
    
private:
    void generateOutputFile(const std::vector<InterfaceInfo>& interfaces, 
                           const std::string& outputFile) {
        std::ofstream out(outputFile);
        if (!out.is_open()) {
            throw std::runtime_error("Cannot create output file: " + outputFile);
        }
        
        // Заголовок файла
        out << "// Auto-generated adapter code\n";
        out << "// DO NOT EDIT MANUALLY\n\n";
        out << "#include \"IoC.hpp\"\n";
        out << "#include \"ICommand.hpp\"\n\n";
        
        // Добавление include для каждого интерфейса
        std::set<std::string> includes;
        for (const auto& interface : interfaces) {
            includes.insert(interface.className + ".hpp");
        }
        
        for (const auto& include : includes) {
            out << "#include \"" << include << "\"\n";
        }
        out << "\n";
        
        // Генерация адаптеров
        out << "namespace AutoGenerated {\n\n";
        
        for (const auto& interface : interfaces) {
            out << generator_.generateAdapter(interface) << "\n\n";
        }
        
        // Функция регистрации всех адаптеров
        generateRegistrationFunction(interfaces, out);
        
        out << "} // namespace AutoGenerated\n";
        
        std::cout << "Generated " << interfaces.size() 
                  << " adapters in " << outputFile << std::endl;
    }
    
    void generateRegistrationFunction(const std::vector<InterfaceInfo>& interfaces,
                                    std::ofstream& out) {
        out << "class AdapterRegistration {\n";
        out << "public:\n";
        out << "    static void registerAll() {\n";
        
        for (const auto& interface : interfaces) {
            std::string adapterName = interface.className.substr(1) + "Adapter";
            out << "        IoC::resolve<ICommand>(\"IoC.Register\",\n";
            out << "            \"Adapter:" << interface.className << "\",\n";
            out << "            [](std::shared_ptr<void> obj) -> std::shared_ptr<" 
                << interface.className << "> {\n";
            out << "                auto gameObj = std::static_pointer_cast<UObject>(obj);\n";
            out << "                return std::make_shared<" << adapterName 
                << ">(gameObj.get());\n";
            out << "            }\n";
            out << "        )->execute();\n\n";
        }
        
        out << "    }\n";
        out << "};\n\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_dir> <output_file>" << std::endl;
        return 1;
    }
    
    try {
        AdapterGenerator generator;
        generator.generateAdapters(argv[1], argv[2]);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

## CMake интеграция

```cmake
# CMakeLists.txt для генератора адаптеров

cmake_minimum_required(VERSION 3.16)
project(AdapterGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Сборка генератора адаптеров
add_executable(adapter_generator
    src/main.cpp
    src/SimpleInterfaceParser.cpp
    src/AdapterCodeGenerator.cpp
    src/AdapterGenerator.cpp
)

target_include_directories(adapter_generator PRIVATE include)

# Установка генератора
install(TARGETS adapter_generator DESTINATION bin)

# Макрос для использования в других проектах
function(generate_adapters TARGET_NAME)
    set(options "")
    set(oneValueArgs INPUT_DIR OUTPUT_FILE)
    set(multiValueArgs INTERFACE_HEADERS)
    cmake_parse_arguments(GEN_ADAPTERS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Поиск заголовочных файлов с интерфейсами
    if(NOT GEN_ADAPTERS_INTERFACE_HEADERS)
        file(GLOB_RECURSE GEN_ADAPTERS_INTERFACE_HEADERS 
             "${GEN_ADAPTERS_INPUT_DIR}/*Interface*.hpp"
             "${GEN_ADAPTERS_INPUT_DIR}/I*.hpp")
    endif()
    
    # Создание custom command для генерации адаптеров
    add_custom_command(
        OUTPUT ${GEN_ADAPTERS_OUTPUT_FILE}
        COMMAND adapter_generator ${GEN_ADAPTERS_INPUT_DIR} ${GEN_ADAPTERS_OUTPUT_FILE}
        DEPENDS ${GEN_ADAPTERS_INTERFACE_HEADERS} adapter_generator
        COMMENT "Generating interface adapters for ${TARGET_NAME}"
        VERBATIM
    )
    
    # Добавление сгенерированного файла к целевому объекту
    target_sources(${TARGET_NAME} PRIVATE ${GEN_ADAPTERS_OUTPUT_FILE})
    
    # Очистка при make clean
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${GEN_ADAPTERS_OUTPUT_FILE})
endfunction()

# Пример использования в основном проекте:
# generate_adapters(MyGame 
#     INPUT_DIR ${CMAKE_SOURCE_DIR}/include
#     OUTPUT_FILE ${CMAKE_BINARY_DIR}/generated/adapters.cpp
# )
```

## Пример тестирования

```cpp
#include <gtest/gtest.h>
#include "SimpleInterfaceParser.hpp"
#include "AdapterCodeGenerator.hpp"

class AdapterGeneratorTest : public ::testing::Test {
protected:
    SimpleInterfaceParser parser_;
    AdapterCodeGenerator generator_;
};

TEST_F(AdapterGeneratorTest, ParsesSimpleInterface) {
    std::string testInterface = R"(
        class IMovable {
        public:
            virtual Vector getPosition() const = 0;
            virtual void setPosition(const Vector& pos) = 0;
            virtual Vector getVelocity() const = 0;
        };
    )";
    
    auto interfaces = parser_.parseContent(testInterface);
    
    ASSERT_EQ(interfaces.size(), 1);
    EXPECT_EQ(interfaces[0].className, "IMovable");
    EXPECT_EQ(interfaces[0].methods.size(), 3);
    EXPECT_EQ(interfaces[0].methods[0].name, "getPosition");
    EXPECT_TRUE(interfaces[0].methods[0].isConst);
}

TEST_F(AdapterGeneratorTest, GeneratesCorrectAdapterCode) {
    InterfaceInfo interface;
    interface.className = "IMovable";
    interface.namespaceName = "";
    
    MethodInfo getPos;
    getPos.name = "getPosition";
    getPos.returnType = "Vector";
    getPos.isConst = true;
    getPos.isPureVirtual = true;
    interface.methods.push_back(getPos);
    
    std::string result = generator_.generateAdapter(interface);
    
    EXPECT_NE(result.find("class AutoGenerated::MovableAdapter"), std::string::npos);
    EXPECT_NE(result.find("Vector getPosition()"), std::string::npos);
    EXPECT_NE(result.find("IoC::resolve<Vector>"), std::string::npos);
    EXPECT_NE(result.find("Position.get"), std::string::npos);
}

TEST_F(AdapterGeneratorTest, HandlesVoidMethods) {
    InterfaceInfo interface;
    interface.className = "IFinishable";
    
    MethodInfo finish;
    finish.name = "finish";
    finish.returnType = "void";
    finish.isPureVirtual = true;
    interface.methods.push_back(finish);
    
    std::string result = generator_.generateAdapter(interface);
    
    EXPECT_NE(result.find("void finish()"), std::string::npos);
    EXPECT_NE(result.find("IoC::resolve<ICommand>"), std::string::npos);
    EXPECT_NE(result.find("finish\", obj_)"), std::string::npos);
}
```

Этот пример демонстрирует базовую реализацию генератора адаптеров, который:

1. **Парсит C++ код** с помощью регулярных выражений
2. **Извлекает информацию** об интерфейсах и методах  
3. **Генерирует код адаптеров** по шаблонам
4. **Интегрируется с CMake** для автоматической сборки
5. **Включает тесты** для проверки функциональности

Для продакшн-использования рекомендуется использовать более мощные инструменты парсинга, такие как libclang или clang-ast-matchers.