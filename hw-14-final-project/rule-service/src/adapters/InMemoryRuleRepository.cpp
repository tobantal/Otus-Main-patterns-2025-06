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
    
    Rule rule1{"promo", "https://example.com/promo", "country == \"RU\""};
    create(rule1);

    Rule rule2{"docs", "https://docs.example.com", "browser == \"chrome\" AND country == \"US\""};
    create(rule2);

    Rule rule3{"blog", "https://blog.example.com", "device == \"mobile\" OR device == \"tablet\""};
    create(rule3);

    Rule rule4{"safari_ru", "https://ru.example.com/safari", "browser == \"safari\" AND country == \"RU\""};
    create(rule4);

    Rule rule5{"night_22", "https://example.com/night22", "date == \"2025-11-22\""};
    create(rule5);

    Rule rule6{"ip_internal", "https://intranet.example.com", "ip == \"10.0.0.1\""};
    create(rule6);

    Rule rule7{"holiday_fixed", "https://holiday.example.com", "date == \"2025-12-31\""};
    create(rule7);

    Rule rule8{"edge_de", "https://de.example.com/edge", "browser == \"edge\" AND country == \"DE\""};
    create(rule8);

    Rule rule9{"mobile_us", "https://us.example.com/mobile", "device == \"mobile\" AND country == \"US\""};
    create(rule9);

    Rule rule10{"china", "https://cn.example.com", "country == \"CN\""};
    create(rule10);

    Rule rule11{"chrome_fr_or_tablet", "https://fr.example.com/alt", "browser == \"chrome\" AND country == \"FR\" OR device == \"tablet\""};
    create(rule11);

    Rule rule12{"unknown_ua", "https://fallback.example.com", "browser == \"unknown\""};
    create(rule12);

    Rule rule13{"premium_app", "https://premium.example.com", "header.User-Agent == \"PremiumApp\""};
    create(rule13);

    std::cout << "[InMemoryRuleRepository] Initialized with " << totalCount_ << " test rules" << std::endl;
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
