/**
 * @file InterfaceInfo.cpp
 * @brief Реализация структуры InterfaceInfo
 * @author Anton Tobolkin
 * @version 1.0
 */

#include "../../include/models/InterfaceInfo.hpp"
#include "../../include/utils/StringUtils.hpp"

std::string InterfaceInfo::getFullName() const {
    if (namespaceName.empty()) {
        return className;
    }
    return namespaceName + "::" + className;
}

std::string InterfaceInfo::getAdapterName() const {
    std::string name = className;
    // Если имя начинается с 'I', убираем его
    if (StringUtils::startsWith(name, "I") && name.length() > 1) {
        name = name.substr(1);
    }
    return name + "Adapter";
}

bool InterfaceInfo::isValidInterface() const {
    if (methods.empty()) {
        return false;
    }

    // Проверяем, что все методы чисто виртуальные
    for (const auto& method : methods) {
        if (!method.isPureVirtual) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> InterfaceInfo::getRequiredIncludes() const {
    std::vector<std::string> includes;
    
    // Добавляем основные include
    includes.push_back("memory");
    includes.push_back("stdexcept");
    
    // Добавляем include для интерфейса
    if (!sourceFile.empty()) {
        includes.push_back(sourceFile);
    }
    
    // Добавляем IoC include
    includes.push_back("IoC.hpp");
    includes.push_back("ICommand.hpp");
    
    // Добавляем includes для кастомных реализаций
    for (const auto& method : methods) {
        if (method.hasCustomImplementation()) {
            // Предполагаем, что кастомные реализации находятся в отдельных файлах
            std::string customInclude = method.customImpl.className + ".hpp";
            includes.push_back(customInclude);
        }
    }
    
    return includes;
}