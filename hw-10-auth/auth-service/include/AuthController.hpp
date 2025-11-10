#pragma once

#include <httplib.h>
#include <memory>
#include "GameRegistry.hpp"
#include <IJWTGenerator.hpp>

/**
 * @brief Контроллер для сервиса авторизации
 *
 * Содержит ВСЕ HTTP endpoints для Auth Service
 * Это единственное место где регистрируются эндпоинты!
 * Используется как в main.cpp, так и в тестах
 */
class AuthController : public std::enable_shared_from_this<AuthController> 
{
private:
    std::shared_ptr<GameRegistry> registry_;
    std::shared_ptr<IJWTGenerator> jwtGenerator_;

public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    AuthController(
        std::shared_ptr<GameRegistry> registry,
        std::shared_ptr<IJWTGenerator> jwtGenerator);

    /**
     * @brief Регистрирует все endpoints на сервере
     *
     * КЛЮЧЕВОЙ МЕТОД - здесь определяются все эндпоинты
     * Если меняем endpoint здесь, то меняется везде:
     * - в main.cpp
     * - в интеграционных тестах
     */
    void registerRoutes(httplib::Server &server);

private:
    void handleCreateGame(const httplib::Request &req, httplib::Response &res);
    void handleIssueToken(const httplib::Request &req, httplib::Response &res);
};