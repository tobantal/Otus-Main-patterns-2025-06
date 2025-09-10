// FileUtils.cpp
#include "../../include/utils/FileUtils.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

std::string FileUtils::readFile(const std::string& filePath) {
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open file for reading: " + filePath);
    }
    std::ostringstream contents;
    contents << in.rdbuf();
    if (in.fail() && !in.eof()) {
        throw std::runtime_error("Error while reading file: " + filePath);
    }
    return contents.str();
}

void FileUtils::writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream out(filePath, std::ios::out | std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }
    out << content;
    if (!out) {
        throw std::runtime_error("Error while writing to file: " + filePath);
    }
}

std::vector<std::string> FileUtils::findFilesWithExtension(
    const std::string& directory,
    const std::string& extension,
    bool recursive)
{
    std::vector<std::string> result;
    std::filesystem::directory_options opts = recursive
        ? std::filesystem::directory_options::follow_directory_symlink
        : std::filesystem::directory_options::none;
    std::filesystem::recursive_directory_iterator it(directory, opts), end;
    std::filesystem::directory_iterator it_nonrec(directory), end_nonrec;
    if (recursive) {
        for (; it != end; ++it) {
            if (!it->is_directory() && it->path().extension() == extension) {
                result.push_back(it->path().string());
            }
        }
    } else {
        for (; it_nonrec != end_nonrec; ++it_nonrec) {
            if (!it_nonrec->is_directory() && it_nonrec->path().extension() == extension) {
                result.push_back(it_nonrec->path().string());
            }
        }
    }
    return result;
}

bool FileUtils::fileExists(const std::string& filePath) {
    return std::filesystem::exists(filePath);
}

void FileUtils::createDirectories(const std::string& dirPath) {
    std::error_code ec;
    if (!std::filesystem::create_directories(dirPath, ec) && ec) {
        throw std::runtime_error("Cannot create directories: " + dirPath + " (" + ec.message() + ")");
    }
}

std::string FileUtils::getFileName(const std::string& path) {
    if (path.empty()) return {};
    // Найти позицию последнего символа '/' или '\'
    size_t pos1 = path.find_last_of('/');
    size_t pos2 = path.find_last_of('\\');
    size_t pos = std::string::npos;
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        pos = std::max(pos1, pos2);
    } else if (pos1 != std::string::npos) {
        pos = pos1;
    } else if (pos2 != std::string::npos) {
        pos = pos2;
    }
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

std::string FileUtils::getFileExtension(const std::string& filePath) {
    return std::filesystem::path(filePath).extension().string();
}
