#pragma once

#include <string>

/**
 * @file ServerConfig.hpp
 * @brief Конфигурация сервера
 * @author Anton Tobolkin
 */

/**
 * @struct ServerConfig
 * @brief Параметры запуска сервера
 */
struct ServerConfig
{
    std::string host;
    int port;
    
    ServerConfig(const std::string& h, int p)
        : host(h), port(p) {}
};