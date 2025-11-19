#pragma once

#include "BoostBeastApplication.hpp"
#include "IHttpHandler.hpp"
#include <map>
#include <memory>
#include <string>

/**
 * @file RedirectServiceApp.hpp
 * @brief Главный класс приложения Redirect Service
 * @author Anton Tobolkin
 */
class RedirectServiceApp : public BoostBeastApplication
{
public:
    RedirectServiceApp() = default;
    ~RedirectServiceApp() = default;

    /**
     * @brief Загрузить конфигурацию приложения
     * @param argc Количество аргументов командной строки
     * @param argv Аргументы командной строки
     */
    void loadEnvironment(int argc, char* argv[]) override;

    /**
     * @brief Настроить DI-контейнер и зарегистрировать хендлеры
     */
    void configureInjection();

protected:
    /**
     * @brief Обработать HTTP-запрос
     * @param beast_req Beast HTTP запрос
     * @param beast_res Beast HTTP ответ
     * @param clientIp IP-адрес клиента
     */
    void handleRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& beast_req,
        boost::beast::http::response<boost::beast::http::string_body>& beast_res,
        const std::string& clientIp) override;

private:
    std::map<std::string, std::shared_ptr<IHttpHandler>> handlers_;
};