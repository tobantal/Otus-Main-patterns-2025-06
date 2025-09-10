/**
 * @file CustomImplementation.cpp
 * @brief Реализация структуры CustomImplementation
 * @author Anton Tobolkin
 * @version 1.0
 */

#include "../../include/models/CustomImplementation.hpp"
#include "../../include/utils/StringUtils.hpp"

bool CustomImplementation::isValid() const {
    return hasCustom && 
           !className.empty() && 
           !methodName.empty() && 
           !fullReference.empty();
}

bool CustomImplementation::parseFromAnnotation(const std::string& annotation) {
    // Ожидаемый формат: "ClassName::methodName"
    std::vector<std::string> parts = StringUtils::split(annotation, "::");
    if (parts.size() < 2) {
        return false;
    }

    int size = parts.size();
    // локальное имя файла с namespace 
    std::string _className = StringUtils::trim(parts[0]);

    for (int i = 1; i < size - 1; i++) {
        std::string subPart = StringUtils::trim(parts[i]);
        if (subPart.empty()) {
            return false;
        }
        _className = _className + "::" + subPart;
    }

    className = _className;
    methodName = StringUtils::trim(parts[size -1]);

    if (className.empty() || methodName.empty()) {
        return false;
    }

    fullReference = className + "::" + methodName;
    hasCustom = true;
    return true;
}

std::string CustomImplementation::getFullReference() const {
    if (!isValid()) {
        return "";
    }
    return className + "::" + methodName;
}