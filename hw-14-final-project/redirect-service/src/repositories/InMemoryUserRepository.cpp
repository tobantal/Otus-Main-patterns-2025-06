#include "repositories/InMemoryUserRepository.hpp"
#include <iostream>

/**
 * @file InMemoryUserRepository.cpp
 * @brief Реализация in-memory репозитория
 * @author Anton Tobolkin
 */

InMemoryUserRepository::InMemoryUserRepository()
{
    std::cout << "[Repository] InMemoryUserRepository created" << std::endl;
    
    // Добавляем тестовые данные
    User user1{1, "Alice", "alice@example.com"};
    User user2{2, "Bob", "bob@example.com"};
    User user3{3, "Charlie", "charlie@example.com"};
    
    users_[1] = user1;
    users_[2] = user2;
    users_[3] = user3;
    
    std::cout << "[Repository] Pre-loaded " << users_.size() << " test users" << std::endl;
}

std::optional<User> InMemoryUserRepository::findById(int id)
{
    std::cout << "[Repository] Finding user by id: " << id << std::endl;
    
    // Ищем пользователя в map
    auto it = users_.find(id);
    if (it != users_.end())
    {
        std::cout << "[Repository] User found: " << it->second.name << std::endl;
        return it->second;
    }
    
    std::cout << "[Repository] User not found" << std::endl;
    return std::nullopt;
}

void InMemoryUserRepository::save(const User& user)
{
    std::cout << "[Repository] Saving user: " << user.name << " (id=" << user.id << ")" << std::endl;
    
    // Сохраняем пользователя в map
    users_[user.id] = user;
    
    std::cout << "[Repository] User saved successfully" << std::endl;
}