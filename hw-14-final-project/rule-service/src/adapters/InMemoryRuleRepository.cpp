#include "adapters/InMemoryRuleRepository.hpp"
#include <iostream>

/**
 * @file InMemoryRuleRepository.cpp
 * @brief Реализация in-memory репозитория правил
 * @author Anton Tobolkin
 */

InMemoryRuleRepository::InMemoryRuleRepository()
    : totalCount_(0)
{
    std::cout << "[InMemoryRuleRepository] Created" << std::endl;
    
    // Добавляем тестовые правила для демонстрации (10 правил с различными условиями)
    
    // 1. Простое условие (1 предикат)
    Rule rule1{"promo", "https://example.com/promo", "country == \"RU\""};
    create(rule1);
    
    // 2. AND с 2 предикатами
    Rule rule2{"docs", "https://docs.example.com", "browser == \"chrome\" AND country == \"US\""};
    create(rule2);
    
    // 3. OR с 2 предикатами
    Rule rule3{"blog", "https://blog.example.com", "device == \"mobile\" OR device == \"tablet\""};
    create(rule3);
    
    // 4. AND с 3 предикатами
    Rule rule4{"sale", "https://sale.example.com", "country == \"RU\" AND browser == \"firefox\" AND device == \"desktop\""};
    create(rule4);
    
    // 5. OR с 3 предикатами
    Rule rule5{"news", "https://news.example.com", "language == \"ru\" OR language == \"en\" OR language == \"de\""};
    create(rule5);
    
    // 6. Комбинация AND и OR (2 предиката)
    Rule rule6{"shop", "https://shop.example.com", "country == \"US\" AND (device == \"mobile\" OR device == \"tablet\")"};
    create(rule6);
    
    // 7. Простое условие с датой
    Rule rule7{"archive", "https://archive.example.com", "date < \"2025-01-01\""};
    create(rule7);
    
    // 8. AND с датой и страной (2 предиката)
    Rule rule8{"special", "https://special.example.com", "country == \"DE\" AND date >= \"2025-11-01\""};
    create(rule8);
    
    // 9. OR с браузерами (3 предиката)
    Rule rule9{"beta", "https://beta.example.com", "browser == \"chrome\" OR browser == \"firefox\" OR browser == \"safari\""};
    create(rule9);
    
    // 10. Комплексное условие (3 предиката с AND/OR)
    Rule rule10{"premium", "https://premium.example.com", "country == \"RU\" AND (browser == \"chrome\" OR browser == \"edge\") AND device == \"desktop\""};
    create(rule10);
    
    std::cout << "[InMemoryRuleRepository] Initialized with " << totalCount_ << " test rules (with complex conditions)" << std::endl;
}

InMemoryRuleRepository::~InMemoryRuleRepository()
{
    std::cout << "[InMemoryRuleRepository] Destroyed" << std::endl;
}

bool InMemoryRuleRepository::create(const Rule& rule)
{
    std::cout << "[InMemoryRuleRepository] Creating rule: " << rule.shortId << std::endl;
    
    // Проверяем, не существует ли уже
    if (rules_.contains(rule.shortId))
    {
        std::cout << "[InMemoryRuleRepository] Rule already exists: " << rule.shortId << std::endl;
        return false;
    }
    
    // Создаём shared_ptr для хранения в ThreadSafeMap
    auto rulePtr = std::make_shared<Rule>(rule);
    rules_.insert(rule.shortId, rulePtr);
    totalCount_++;
    
    std::cout << "[InMemoryRuleRepository] Rule created successfully" << std::endl;
    return true;
}

std::optional<Rule> InMemoryRuleRepository::findById(const std::string& shortId)
{
    std::cout << "[InMemoryRuleRepository] Finding rule: " << shortId << std::endl;
    
    auto rulePtr = rules_.find(shortId);
    if (!rulePtr)
    {
        std::cout << "[InMemoryRuleRepository] Rule not found: " << shortId << std::endl;
        return std::nullopt;
    }
    
    std::cout << "[InMemoryRuleRepository] Rule found: " << rulePtr->targetUrl << std::endl;
    return *rulePtr;  // Разыменовываем shared_ptr в Rule
}

PaginatedRules InMemoryRuleRepository::findAll(int page, int pageSize)
{
    std::cout << "[InMemoryRuleRepository] Listing all rules: page=" << page 
              << ", pageSize=" << pageSize << std::endl;
    
    // Получаем все правила из ThreadSafeMap
    auto allRulesPtr = rules_.getAll();
    
    // Разыменовываем shared_ptr в вектор Rule
    std::vector<Rule> allRules;
    allRules.reserve(allRulesPtr.size());
    for (const auto& rulePtr : allRulesPtr)
    {
        allRules.push_back(*rulePtr);
    }
    
    // Вычисляем пагинацию
    int totalCount = static_cast<int>(allRules.size());
    int startIndex = (page - 1) * pageSize;
    int endIndex = std::min(startIndex + pageSize, totalCount);
    
    // Извлекаем нужный диапазон
    std::vector<Rule> pageRules;
    if (startIndex < totalCount)
    {
        pageRules.assign(allRules.begin() + startIndex, allRules.begin() + endIndex);
    }
    
    PaginatedRules result;
    result.rules = pageRules;
    result.page = page;
    result.pageSize = pageSize;
    result.totalCount = totalCount;
    
    std::cout << "[InMemoryRuleRepository] Returning " << pageRules.size() 
              << " rules (total: " << totalCount << ")" << std::endl;
    
    return result;
}

bool InMemoryRuleRepository::update(const std::string& shortId, const Rule& rule)
{
    std::cout << "[InMemoryRuleRepository] Updating rule: " << shortId << std::endl;
    
    // Проверяем существование
    if (!rules_.contains(shortId))
    {
        std::cout << "[InMemoryRuleRepository] Rule not found for update: " << shortId << std::endl;
        return false;
    }
    
    // Обновляем (перезаписываем)
    auto rulePtr = std::make_shared<Rule>(rule);
    rules_.insert(shortId, rulePtr);
    
    std::cout << "[InMemoryRuleRepository] Rule updated successfully" << std::endl;
    return true;
}

bool InMemoryRuleRepository::deleteById(const std::string& shortId)
{
    std::cout << "[InMemoryRuleRepository] Deleting rule: " << shortId << std::endl;
    
    // Проверяем существование
    if (!rules_.contains(shortId))
    {
        std::cout << "[InMemoryRuleRepository] Rule not found for delete: " << shortId << std::endl;
        return false;
    }

    rules_.remove(shortId);
    totalCount_--;
    
    std::cout << "[InMemoryRuleRepository] Rule deleted successfully: " << shortId << std::endl;
    return true;
}
