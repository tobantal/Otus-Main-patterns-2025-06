#include "adapters/PostgreSQLRuleRepository.hpp"
#include <iostream>
#include <stdexcept>
#include <settings/DbSettings.hpp>

/**
 * @file PostgreSQLRuleRepository.cpp
 * @brief Реализация адаптера PostgreSQL
 * @author Anton Tobolkin
 */

PostgreSQLRuleRepository::PostgreSQLRuleRepository(std::shared_ptr<IDbSettings> dbSettings)
{
    // Формируем строку подключения
    std::string connectionString =
        "host=" + dbSettings->getHost() +
        " port=" + std::to_string(dbSettings->getPort()) +
        " dbname=" + dbSettings->getName() +
        " user=" + dbSettings->getUser() +
        " password=" + dbSettings->getPassword();

    try
    {
        std::cout << "[PostgreSQLRuleRepository] Connecting to database..." << std::endl;
        connection_ = std::make_unique<pqxx::connection>(connectionString);

        if (connection_->is_open())
        {
            std::cout << "[PostgreSQLRuleRepository] Connected to: "
                      << connection_->dbname() << std::endl;
        }
        else
        {
            throw std::runtime_error("Failed to open database connection");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Connection error: " << e.what() << std::endl;
        throw;
    }
}

PostgreSQLRuleRepository::~PostgreSQLRuleRepository()
{
    if (connection_ && connection_->is_open())
    {
        connection_->close();
        std::cout << "[PostgreSQLRuleRepository] Database connection closed" << std::endl;
    }
}

bool PostgreSQLRuleRepository::create(const Rule &rule)
{
    try
    {
        std::cout << "[PostgreSQLRuleRepository] Creating rule: " << rule.shortId << std::endl;

        pqxx::work txn(*connection_);

        // Подготовленный запрос для вставки
        std::string query =
            "INSERT INTO rules (short_id, target_url, condition) "
            "VALUES ($1, $2, $3)";

        txn.exec_params(query, rule.shortId, rule.targetUrl, rule.condition);
        txn.commit();

        std::cout << "[PostgreSQLRuleRepository] Rule created successfully" << std::endl;
        return true;
    }
    catch (const pqxx::unique_violation &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Rule already exists: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Create error: " << e.what() << std::endl;
        return false;
    }
}

std::optional<Rule> PostgreSQLRuleRepository::findById(const std::string &shortId)
{
    try
    {
        std::cout << "[PostgreSQLRuleRepository] Finding rule: " << shortId << std::endl;

        pqxx::work txn(*connection_);

        std::string query =
            "SELECT short_id, target_url, condition, created_at, updated_at "
            "FROM rules WHERE short_id = $1";

        pqxx::result result = txn.exec_params(query, shortId);

        if (result.empty())
        {
            std::cout << "[PostgreSQLRuleRepository] Rule not found" << std::endl;
            return std::nullopt;
        }

        // Извлекаем данные из первой строки
        auto row = result[0];

        RuleEntity entity{
            row["short_id"].as<std::string>(),
            row["target_url"].as<std::string>(),
            row["condition"].as<std::string>(),
            row["created_at"].as<std::string>(),
            row["updated_at"].as<std::string>()};

        std::cout << "[PostgreSQLRuleRepository] Rule found" << std::endl;
        return entityToRule(entity);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Find error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

PaginatedRules PostgreSQLRuleRepository::findAll(int page, int pageSize)
{
    try
    {
        std::cout << "[PostgreSQLRuleRepository] Finding all rules, page="
                  << page << ", size=" << pageSize << std::endl;

        pqxx::work txn(*connection_);

        // Вычисляем OFFSET
        int offset = (page - 1) * pageSize;

        // Запрос с пагинацией
        std::string query =
            "SELECT short_id, target_url, condition, created_at, updated_at "
            "FROM rules ORDER BY created_at DESC LIMIT $1 OFFSET $2";

        pqxx::result result = txn.exec_params(query, pageSize, offset);

        // Получаем общее количество
        std::string countQuery = "SELECT COUNT(*) FROM rules";
        pqxx::result countResult = txn.exec(countQuery);
        int totalCount = countResult[0][0].as<int>();

        // Конвертируем результаты
        std::vector<Rule> rules;
        for (const auto &row : result)
        {
            RuleEntity entity{
                row["short_id"].as<std::string>(),
                row["target_url"].as<std::string>(),
                row["condition"].as<std::string>(),
                row["created_at"].as<std::string>(),
                row["updated_at"].as<std::string>()};
            rules.push_back(entityToRule(entity));
        }

        std::cout << "[PostgreSQLRuleRepository] Found " << rules.size() << " rules" << std::endl;

        return PaginatedRules{
            rules,
            totalCount,
            page,
            pageSize};
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] FindAll error: " << e.what() << std::endl;
        return PaginatedRules{{}, 0, page, pageSize};
    }
}

bool PostgreSQLRuleRepository::update(const std::string &shortId, const Rule &rule)
{
    try
    {
        std::cout << "[PostgreSQLRuleRepository] Updating rule: " << shortId << std::endl;

        pqxx::work txn(*connection_);

        std::string query =
            "UPDATE rules SET target_url = $1, condition = $2, updated_at = CURRENT_TIMESTAMP "
            "WHERE short_id = $3";

        pqxx::result result = txn.exec_params(query, rule.targetUrl, rule.condition, shortId);
        txn.commit();

        // Проверяем, была ли обновлена хотя бы одна строка
        bool updated = result.affected_rows() > 0;

        if (updated)
        {
            std::cout << "[PostgreSQLRuleRepository] Rule updated successfully" << std::endl;
        }
        else
        {
            std::cout << "[PostgreSQLRuleRepository] Rule not found for update" << std::endl;
        }

        return updated;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Update error: " << e.what() << std::endl;
        return false;
    }
}

bool PostgreSQLRuleRepository::deleteById(const std::string &shortId)
{
    try
    {
        std::cout << "[PostgreSQLRuleRepository] Deleting rule: " << shortId << std::endl;

        pqxx::work txn(*connection_);

        std::string query = "DELETE FROM rules WHERE short_id = $1";

        pqxx::result result = txn.exec_params(query, shortId);
        txn.commit();

        // Проверяем, была ли удалена хотя бы одна строка
        bool deleted = result.affected_rows() > 0;

        if (deleted)
        {
            std::cout << "[PostgreSQLRuleRepository] Rule deleted successfully" << std::endl;
        }
        else
        {
            std::cout << "[PostgreSQLRuleRepository] Rule not found for deletion" << std::endl;
        }

        return deleted;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[PostgreSQLRuleRepository] Delete error: " << e.what() << std::endl;
        return false;
    }
}

Rule PostgreSQLRuleRepository::entityToRule(const RuleEntity &entity) const
{
    // Конвертируем RuleEntity (с timestamps) в Rule (без timestamps)
    return Rule{
        entity.shortId,
        entity.targetUrl,
        entity.condition};
}

RuleEntity PostgreSQLRuleRepository::ruleToEntity(const Rule &rule) const
{
    // Конвертируем Rule в RuleEntity (timestamps будут установлены БД)
    return RuleEntity{
        rule.shortId,
        rule.targetUrl,
        rule.condition,
        "", // createdAt устанавливается БД
        ""  // updatedAt устанавливается БД
    };
}