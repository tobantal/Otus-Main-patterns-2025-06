#include "handlers/GetUserHandler.hpp"
#include <iostream>
#include <sstream>

/**
 * @file GetUserHandler.cpp
 * @brief Реализация хендлера получения пользователя
 * @author Anton Tobolkin
 */

GetUserHandler::GetUserHandler(std::shared_ptr<IUserService> userService)
    : userService_(userService)
{
    std::cout << "[GetUserHandler] Handler created with injected service" << std::endl;
}

void GetUserHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[GetUserHandler] Handling request: " << req.getMethod() << " " << req.getPath() << std::endl;
    
    // Извлекаем ID из пути
    int id = extractUserId(req.getPath());
    
    if (id == -1)
    {
        std::cout << "[GetUserHandler] Invalid user ID in path" << std::endl;
        res.setStatus(400);
        res.setHeader("Content-Type", "application/json");
        res.setBody("{\"error\": \"Invalid user ID\"}");
        return;
    }
    
    std::cout << "[GetUserHandler] Looking for user with id: " << id << std::endl;
    
    // Получаем пользователя через сервис
    auto user = userService_->getUser(id);
    
    if (!user.has_value())
    {
        std::cout << "[GetUserHandler] User not found" << std::endl;
        res.setStatus(404);
        res.setHeader("Content-Type", "application/json");
        res.setBody("{\"error\": \"User not found\"}");
        return;
    }
    
    // Формируем JSON ответ
    std::ostringstream json;
    json << "{"
         << "\"id\": " << user->id << ", "
         << "\"name\": \"" << user->name << "\", "
         << "\"email\": \"" << user->email << "\""
         << "}";
    
    std::cout << "[GetUserHandler] Returning user: " << json.str() << std::endl;
    
    res.setStatus(200);
    res.setHeader("Content-Type", "application/json");
    res.setBody(json.str());
}

int GetUserHandler::extractUserId(const std::string& path) const
{
    // Ожидаем путь вида: /api/users/123
    std::string prefix = "/api/users/";
    
    if (path.find(prefix) != 0)
    {
        return -1;
    }
    
    std::string idStr = path.substr(prefix.length());
    
    // Убираем query string если есть
    size_t queryPos = idStr.find('?');
    if (queryPos != std::string::npos)
    {
        idStr = idStr.substr(0, queryPos);
    }
    
    try
    {
        return std::stoi(idStr);
    }
    catch (const std::exception&)
    {
        return -1;
    }
}