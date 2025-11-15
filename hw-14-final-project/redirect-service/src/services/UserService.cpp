#include "services/UserService.hpp"
#include <iostream>
#include <atomic>

/**
 * @file UserService.cpp
 * @brief Реализация сервиса пользователей
 * @author Anton Tobolkin
 */

UserService::UserService(std::shared_ptr<IUserRepository> repository)
    : repository_(repository)
{
    std::cout << "[Service] UserService created with injected repository" << std::endl;
}

std::optional<User> UserService::getUser(int id)
{
    std::cout << "[Service] getUser called with id: " << id << std::endl;
    
    // Делегируем вызов репозиторию
    auto user = repository_->findById(id);
    
    if (user.has_value())
    {
        std::cout << "[Service] User retrieved: " << user->name << std::endl;
    }
    else
    {
        std::cout << "[Service] User not found" << std::endl;
    }
    
    return user;
}

User UserService::createUser(const std::string& name, const std::string& email)
{
    std::cout << "[Service] createUser called: " << name << ", " << email << std::endl;
    
    // Генерируем ID (в реальном приложении это делала бы БД)
    static std::atomic<int> nextId{1000};
    int id = nextId++;
    
    // Создаем пользователя
    User user{id, name, email};
    
    // Сохраняем через репозиторий
    repository_->save(user);
    
    std::cout << "[Service] User created with id: " << id << std::endl;
    
    return user;
}