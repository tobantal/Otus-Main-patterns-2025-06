#pragma once

#include "IUserRepository.hpp"
#include <map>

/**
 * @file InMemoryUserRepository.hpp
 * @brief Заголовок in-memory репозитория
 * @author Anton Tobolkin
 */

/**
 * @class InMemoryUserRepository
 * @brief In-memory хранилище пользователей
 */
class InMemoryUserRepository : public IUserRepository
{
public:
    InMemoryUserRepository();
    
    std::optional<User> findById(int id) override;
    void save(const User& user) override;

private:
    std::map<int, User> users_;
};