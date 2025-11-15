#pragma once

#include <string>
#include <optional>

/**
 * @file IUserRepository.hpp
 * @brief Интерфейс репозитория пользователей
 * @author Anton Tobolkin
 */

/**
 * @struct User
 * @brief Простая структура пользователя
 */
struct User
{
    int id;
    std::string name;
    std::string email;
};

/**
 * @interface IUserRepository
 * @brief Абстрактный интерфейс для работы с хранилищем пользователей
 * 
 * Следует Repository паттерну для абстрагирования от конкретной
 * реализации хранилища данных (InMemory, Database, File и т.д.)
 */
class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    /**
     * @brief Найти пользователя по ID
     * @param id Идентификатор пользователя
     * @return Пользователь или std::nullopt если не найден
     */
    virtual std::optional<User> findById(int id) = 0;

    /**
     * @brief Сохранить пользователя
     * @param user Данные пользователя
     */
    virtual void save(const User& user) = 0;
};