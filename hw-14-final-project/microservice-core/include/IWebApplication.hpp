#pragma once

#include "ServerConfig.hpp"

/**
 * @file IWebApplication.hpp
 * @brief Базовый интерфейс веб-приложения
 * @author Anton Tobolkin
 */

/**
 * @class IWebApplication
 * @brief Интерфейс веб-приложения с Template Method паттерном
 * 
 * Паттерн Template Method:
 * - run() - шаблонный метод (не виртуальный)
 * - configureInjection() - настройка DI
 * - configureRoutes() - настройка маршрутов
 * - start() - запуск сервера
 */
class IWebApplication
{
public:
    /**
     * @brief Конструктор с конфигурацией сервера
     */
    IWebApplication(const ServerConfig& config) 
        : config_(config) {}
    
    virtual ~IWebApplication() = default;

    /**
     * @brief Запустить приложение (Template Method)
     * 
     * Вызывает последовательно:
     * 1. configureInjection()
     * 2. configureRoutes()
     * 3. start()
     */
    void run()
    {
        configureInjection();
        configureRoutes();
        start();
    }

protected:
    /**
     * @brief Настроить DI контейнер (Boost.DI injector)
     */
    virtual void configureInjection() = 0;

    /**
     * @brief Настроить маршруты (регистрация хендлеров)
     */
    virtual void configureRoutes() = 0;

    /**
     * @brief Запустить HTTP сервер
     */
    virtual void start() = 0;

    ServerConfig config_; ///< Конфигурация сервера
};