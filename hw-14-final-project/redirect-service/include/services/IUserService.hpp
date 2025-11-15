#pragma once

#include "../repositories/IUserRepository.hpp"
#include <string>
#include <optional>

/**
 * @file IUserService.hpp
 * @brief Интерфейс сервиса пользователей
 * @author Anton Tobolkin
 */

/**
 * @interface IUserService
 * @brief Абстрактный интерфейс бизнес-логики работы с пользователями
 * 
 * Инкапсулирует бизнес-логику и использует репозиторий для доступа к данным.
 * Следует принципу Dependency Inversion из SOLID.
 */
class IUserService
{
public:
    virtual ~IUserService() = default;

    /**
     * @brief Получить пользователя по ID
     * @param id Идентификатор пользователя
     * @return Пользователь или std::nullopt если не найден
     */
    virtual std::optional<User> getUser(int id) = 0;

    /**
     * @brief Создать нового пользователя
     * @param name Имя пользователя
     * @param email Email пользователя
     * @return Созданный пользователь
     */
    virtual User createUser(const std::string& name, const std::string& email) = 0;
};