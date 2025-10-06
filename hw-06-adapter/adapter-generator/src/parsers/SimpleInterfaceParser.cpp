/**
 * @file SimpleInterfaceParser.cpp
 * @brief Реализация парсера C++ интерфейсов
 * @author Anton Tobolkin
 * @version 1.0
 */

#include "../../include/parsers/SimpleInterfaceParser.hpp"
#include "../../include/utils/FileUtils.hpp"
#include "../../include/utils/StringUtils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

SimpleInterfaceParser::SimpleInterfaceParser()
{
    // Регулярное выражение для поиска классов
    classRegex_ = std::regex(R"(class\s+(\w+)\s*(?:\s*:\s*[^{]*)?\s*\{)");

    // Регулярное выражение для поиска чисто виртуальных методов
    methodRegex_ = std::regex(
        R"(\bvirtual\s+(.+?)\s+(\w+)\s*\(([^)]*)\)\s*(const)?\s*=\s*0\s*;)");

    // Регулярное выражение для поиска namespace
    namespaceRegex_ = std::regex(R"(namespace\s+([A-Za-z_]\w*(?:(?:::\w+)+)?)\s*\{)");

    // Регулярное выражение для поиска кастомных аннотаций
    customImplRegex_ = std::regex(R"(@custom_impl\s+(\w+::\w+))");
}

std::vector<InterfaceInfo> SimpleInterfaceParser::parseFile(const std::string &filePath)
{
    if (!FileUtils::fileExists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath);
    }

    std::string content = FileUtils::readFile(filePath);
    auto interfaces = parseContent(content);

    // Устанавливаем исходный файл для каждого интерфейса
    for (auto &interface : interfaces)
    {
        interface.sourceFile = FileUtils::getFileName(filePath);
    }

    return interfaces;
}

std::vector<InterfaceInfo> SimpleInterfaceParser::parseContent(const std::string &content)
{
    std::vector<InterfaceInfo> interfaces;

    // Поиск всех классов
    std::sregex_iterator classBegin(content.begin(), content.end(), classRegex_);
    std::sregex_iterator classEnd;

    for (auto it = classBegin; it != classEnd; ++it)
    {
        std::smatch match = *it;
        std::string className = match[1].str();

        // Находим содержимое класса
        size_t classPos = match.position();
        size_t braceStart = content.find('{', classPos);
        if (braceStart == std::string::npos)
        {
            continue;
        }

        size_t braceEnd = findMatchingBrace(content, braceStart);
        if (braceEnd == std::string::npos)
        {
            continue;
        }

        std::string classContent = content.substr(braceStart + 1, braceEnd - braceStart - 1);

        // Парсим методы класса
        auto methods = parseMethods(classContent, className);

        // Проверяем, что это интерфейс (только чисто виртуальные методы)
        if (!methods.empty() && hasOnlyPureVirtual(methods))
        {
            InterfaceInfo interface;
            interface.className = className;
            interface.namespaceName = extractNamespace(content, classPos);
            interface.methods = std::move(methods);

            interfaces.push_back(interface);

            std::cout << "Found interface: " << interface.getFullName()
                      << " with " << interface.methods.size() << " methods" << std::endl;
        }
    }

    return interfaces;
}

std::vector<MethodInfo> SimpleInterfaceParser::parseMethods(
    const std::string &classContent,
    const std::string &interfaceName)
{

    std::vector<MethodInfo> methods;

    std::sregex_iterator methodBegin(classContent.begin(), classContent.end(), methodRegex_);
    std::sregex_iterator methodEnd;

    // позиция метода в классе-строке
    size_t methodPos = 0;
    for (auto it = methodBegin; it != methodEnd; ++it)
    {
        std::smatch match = *it;

        MethodInfo method;
        method.returnType = StringUtils::trim(match[1].str());
        method.name = match[2].str();
        method.parameters = parseParameters(match[3].str());
        method.isConst = !match[4].str().empty();
        method.isPureVirtual = true;

        // уменьшаем строку-класс
        // (выкидываем всё, что было до последнего обработанного метода)
        std::string shortStr = classContent.substr(methodPos);
        // Извлекаем комментарии перед методом для поиска аннотаций
        std::string methodComments = extractMethodComments(shortStr, it->position());
        methodPos = it->position();

        method.customImpl = parseCustomImplementation(methodComments);

        methods.push_back(method);

        std::cout << "  Found method: " << method.returnType << " " << method.name;
        if (method.hasCustomImplementation())
        {
            std::cout << " (custom: " << method.customImpl.getFullReference() << ")";
        }
        std::cout << std::endl;
    }

    return methods;
}

std::vector<std::string> SimpleInterfaceParser::parseParameters(const std::string &paramStr)
{
    std::vector<std::string> params;
    if (paramStr.empty())
    {
        return params;
    }

    auto paramList = StringUtils::split(paramStr, ',');
    for (const auto &param : paramList)
    {
        std::string trimmed = StringUtils::trim(param);
        if (!trimmed.empty())
        {
            params.push_back(trimmed);
        }
    }

    return params;
}

bool SimpleInterfaceParser::hasOnlyPureVirtual(const std::vector<MethodInfo> &methods)
{
    return !methods.empty() &&
           std::all_of(methods.begin(), methods.end(),
                       [](const MethodInfo &m)
                       { return m.isPureVirtual; });
}

std::string SimpleInterfaceParser::extractNamespace(const std::string &content, size_t classPos)
{
    auto subs = content.substr(0, classPos);

    std::vector<std::string> namespaces;
    std::sregex_iterator it(content.begin(), content.end(), namespaceRegex_);
    std::sregex_iterator end;
    while (it != end)
    {
        namespaces.push_back((*it)[1].str());
        ++it;
    }

    return namespaces.empty() ? "" : namespaces.back();
}

size_t SimpleInterfaceParser::findMatchingBrace(const std::string &content, size_t start)
{
    int braceCount = 1;
    for (size_t i = start + 1; i < content.size(); ++i)
    {
        if (content[i] == '{')
        {
            braceCount++;
        }
        else if (content[i] == '}')
        {
            braceCount--;
            if (braceCount == 0)
            {
                return i;
            }
        }
    }
    return std::string::npos;
}

CustomImplementation SimpleInterfaceParser::parseCustomImplementation(
    const std::string &commentContent)
{

    CustomImplementation impl;

    std::sregex_iterator customBegin(commentContent.begin(), commentContent.end(), customImplRegex_);
    std::sregex_iterator customEnd;

    for (auto it = customBegin; it != customEnd; ++it)
    {
        std::smatch match = *it;
        std::string annotation = match[1].str();

        if (impl.parseFromAnnotation(annotation))
        {
            break; // Используем первую найденную аннотацию
        }
    }

    return impl;
}

std::string SimpleInterfaceParser::extractMethodComments(
    const std::string &content,
    size_t methodPos)
{

    // Ищем комментарии перед методом (в пределах 500 символов)
    size_t searchStart = (methodPos > 500) ? methodPos - 500 : 0;
    size_t searchLength = methodPos - searchStart;
    if (searchStart >= content.size())
        return "";
    if (searchStart + searchLength > content.size())
    {
        searchLength = content.size() - searchStart;
    }
    std::string searchArea = content.substr(searchStart, searchLength);

    // Находим последний блочный комментарий или группу строчных комментариев
    std::string comments;

    // Поиск блочного комментария /**...*/
    std::regex blockCommentRegex(R"(/\*\*([^*]|\*(?!/))*\*/)");
    std::sregex_iterator blockBegin(searchArea.begin(), searchArea.end(), blockCommentRegex);
    std::sregex_iterator blockEnd;

    for (auto it = blockBegin; it != blockEnd; ++it)
    {
        comments = it->str();
    }

    return comments;
}

std::string SimpleInterfaceParser::trim(const std::string &str)
{
    return StringUtils::trim(str);
}