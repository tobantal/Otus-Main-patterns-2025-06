#pragma once
#include "ports/IRuleRepository.hpp"
#include "domain/Rule.hpp"
#include "domain/PaginatedRules.hpp"
#include <memory>
#include <optional>
#include "ports/IRuleService.hpp"

/**
 * @file RuleService.hpp
 * @brief Бизнес-логика управления правилами
 * @author Anton Tobolkin
 */

/**
 * @class RuleService
 * @brief Сервис для CRUD операций с правилами
 */
class RuleService : public IRuleService
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    // FIXME: добавить ICacheInvalidator
    explicit RuleService(std::shared_ptr<IRuleRepository> repository);

    /**
     * @brief Создать новое правило
     * @param rule Данные правила
     * @return true если успешно создано
     */
    bool create(const Rule &rule);

    /**
     * @brief Получить правило по shortId
     * @param shortId Короткий идентификатор
     * @return Правило или nullopt если не найдено
     */
    std::optional<Rule> findById(const std::string &shortId);

    /**
     * @brief Получить список всех правил
     * @param page Номер страницы (начинается с 1)
     * @param pageSize Размер страницы
     * @return Пагинированный результат
     */
    PaginatedRules findAll(int page, int pageSize);

    /**
     * @brief Обновить правило
     * @param shortId Короткий идентификатор
     * @param rule Новые данные
     * @return true если успешно обновлено
     */
    // FIXME: при вызове методов update и deleteById вызывать ICacheInvalidator, чтоб он делал delete запрос в redeirect-service
    bool update(const std::string &shortId, const Rule &rule);

    /**
     * @brief Удалить правило
     * @param shortId Короткий идентификатор
     * @return true если успешно удалено
     */
    // FIXME: вызывать ICacheInvalidator, чтоб он делал delete запрос в redeirect-service
    bool deleteById(const std::string &shortId);

private:
    std::shared_ptr<IRuleRepository> repository_;
};