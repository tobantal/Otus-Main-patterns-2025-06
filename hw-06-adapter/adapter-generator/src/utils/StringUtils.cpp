/**
 * @file StringUtils.cpp
 * @brief Реализация утилит для работы со строками
 * @author Anton Tobolkin
 * @version 1.0
 */

#include "../../include/utils/StringUtils.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>

std::string StringUtils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;
    size_t delim_len = delimiter.length();

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delim_len;
    }
    result.push_back(str.substr(start));
    return result;
}


std::string StringUtils::replaceAll(const std::string& str, 
                                   const std::string& from, 
                                   const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return str.substr(0, prefix.length()) == prefix;
}

bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

std::string StringUtils::capitalize(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    
    std::string result = str;
    result[0] = std::toupper(result[0]);
    return result;
}

std::string StringUtils::toSnakeCase(const std::string& camelCase) {
    std::string result;
    for (size_t i = 0; i < camelCase.length(); ++i) {
        char c = camelCase[i];
        if (std::isupper(c) && i > 0) {
            result += '_';
        }
        result += std::tolower(c);
    }
    return result;
}