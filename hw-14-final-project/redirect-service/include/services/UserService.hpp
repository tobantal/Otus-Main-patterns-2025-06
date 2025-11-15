#pragma once

#include "IUserService.hpp"
#include "repositories/IUserRepository.hpp"
#include <memory>

/**
 * @file UserService.hpp
 * @brief Заголовок сервиса пользователей
 * @author Anton Tobolkin
 */

/**
 * @class UserService
 * @brief Бизнес-логика работы с пользователями
 */
class UserService : public IUserService
{
public:
    explicit UserService(std::shared_ptr<IUserRepository> repository);
    
    std::optional<User> getUser(int id) override;
    User createUser(const std::string& name, const std::string& email) override;

private:
    std::shared_ptr<IUserRepository> repository_;
};