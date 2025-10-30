#pragma once

#include <httplib.h>
#include <memory>
#include <iostream>

/**
 * @brief Base Application класс для сервисов
 * 
 * Использует Template Method паттерн.
 * Подклассы переопределяют configure() для регистрации компонентов
 * и configureControllers() для регистрации HTTP эндпоинтов
 */
class Application {
public:
    virtual ~Application() = default;

    /**
     * @brief Запускает приложение с HTTP сервером
     */
    void run(const std::string& host, int port) {
        configure();
        httplib::Server server;
        configureControllers(server);
        std::cout << "Starting server on " << host << ":" << port << "..." << std::endl;
        server.listen(host.c_str(), port);
    }

    /**
     * @brief Создает и возвращает HTTP сервер для тестирования
     * Используется в интеграционных тестах
     */
    std::unique_ptr<httplib::Server> createTestServer() {
        configure();
        auto server = std::make_unique<httplib::Server>();
        configureControllers(*server);
        return server;
    }

protected:
    /**
     * @brief Переопределить для регистрации компонентов в IoC
     */
    virtual void configure() = 0;

    /**
     * @brief Переопределить для регистрации HTTP endpoints
     */
    virtual void configureControllers(httplib::Server& server) = 0;
};