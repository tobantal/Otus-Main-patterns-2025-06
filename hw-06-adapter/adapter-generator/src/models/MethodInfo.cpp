/**
 * @file MethodInfo.cpp
 * @brief Реализация структуры MethodInfo
 * @author Anton Tobolkin
 * @version 1.0
 */

#include "../../include/models/MethodInfo.hpp"
#include "../../include/utils/StringUtils.hpp"

bool MethodInfo::isGetter() const {
    return StringUtils::startsWith(name, "get")
        && name.length() > 3
        && parameters.empty();
}

bool MethodInfo::isSetter() const {
    return StringUtils::startsWith(name, "set")
        && name.length() > 3
        && parameters.size() == 1
        && returnType == "void";
}

bool MethodInfo::isVoidMethod() const {
    return returnType == "void" && parameters.empty();
}

std::string MethodInfo::getPropertyName() const {
    if (StringUtils::startsWith(name, "get") || StringUtils::startsWith(name, "set")) {
        return name.substr(3); // Убираем префикс get/set
    }
    return name;
}

bool MethodInfo::hasCustomImplementation() const {
    return customImpl.hasCustom && customImpl.isValid();
}