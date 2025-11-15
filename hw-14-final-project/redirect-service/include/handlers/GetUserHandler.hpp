#pragma once

#include "IHttpHandler.hpp"
#include "services/IUserService.hpp"
#include <memory>

/**
 * @file GetUserHandler.hpp
 * @brief Хендлер для получения пользователя по ID
 * @author Anton Tobolkin
 */

/**
 * @class GetUserHandler
 * @brief Обрабатывает GET запросы для получения пользователя
 * 
 * Эндпоинт: GET /api/users/{id}
 * Регистрируется в IoC с именем "GET:/api/users"
 */
class GetUserHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инжекцией зависимостей
     * @param userService Сервис для работы с пользователями
     */
    explicit GetUserHandler(std::shared_ptr<IUserService> userService);

    /**
     * @brief Обработать запрос получения пользователя
     * @param req HTTP запрос
     * @param res HTTP ответ
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IUserService> userService_;
    
    /**
     * @brief Извлечь ID пользователя из пути
     * @param path Путь запроса (например "/api/users/123")
     * @return ID пользователя или -1 если не удалось извлечь
     */
    int extractUserId(const std::string& path) const;
};