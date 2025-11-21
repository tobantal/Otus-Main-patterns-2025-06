#include <gtest/gtest.h>
#include "services/DSLEvaluator.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * @file DSLEvaluatorTest.cpp
 * @brief Unit-тесты для DSLEvaluator
 * @author Anton Tobolkin
 */

// Вспомогательная функция - получить текущую дату
std::string getCurrentDate()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

// Тест: простое сравнение browser
TEST(DSLEvaluatorTest, SimpleBrowserComparison)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req1{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) Chrome/120.0.0.0"}}};
    RedirectRequest req2{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) Firefox/121.0"}}};
    
    // Chrome должен совпасть
    EXPECT_TRUE(evaluator.evaluate("browser == \"chrome\"", req1));
    
    // Firefox не должен совпасть с chrome
    EXPECT_FALSE(evaluator.evaluate("browser == \"chrome\"", req2));
    
    // Firefox должен совпасть
    EXPECT_TRUE(evaluator.evaluate("browser == \"firefox\"", req2));
}

// Тест: оператор неравенства
TEST(DSLEvaluatorTest, NotEqualOperator)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 Chrome/120.0.0.0"}}};
    
    EXPECT_TRUE(evaluator.evaluate("browser != \"firefox\"", req));
    EXPECT_FALSE(evaluator.evaluate("browser != \"chrome\"", req));
}

// Тест: сравнение дат
TEST(DSLEvaluatorTest, DateComparison)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    
    std::string today = getCurrentDate();
    
    // Текущая дата должна быть меньше будущей даты
    EXPECT_TRUE(evaluator.evaluate("date < \"2030-01-01\"", req));
    
    // Текущая дата должна быть больше прошлой даты
    EXPECT_TRUE(evaluator.evaluate("date > \"2020-01-01\"", req));
    
    // Текущая дата равна себе
    EXPECT_TRUE(evaluator.evaluate("date == \"" + today + "\"", req));
}

// Тест: AND оператор
TEST(DSLEvaluatorTest, AndOperator)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 Chrome/120.0.0.0"}}};
    
    // Оба условия true
    EXPECT_TRUE(evaluator.evaluate("browser == \"chrome\" AND date < \"2030-01-01\"", req));
    
    // Первое true, второе false
    EXPECT_FALSE(evaluator.evaluate("browser == \"chrome\" AND date < \"2020-01-01\"", req));
    
    // Первое false, второе true
    EXPECT_FALSE(evaluator.evaluate("browser == \"firefox\" AND date < \"2030-01-01\"", req));
    
    // Оба false
    EXPECT_FALSE(evaluator.evaluate("browser == \"firefox\" AND date < \"2020-01-01\"", req));
}

// Тест: OR оператор
TEST(DSLEvaluatorTest, OrOperator)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 Chrome/120.0.0.0"}}};
    
    // Оба условия true
    EXPECT_TRUE(evaluator.evaluate("browser == \"chrome\" OR date < \"2030-01-01\"", req));
    
    // Первое true, второе false
    EXPECT_TRUE(evaluator.evaluate("browser == \"chrome\" OR date < \"2020-01-01\"", req));
    
    // Первое false, второе true
    EXPECT_TRUE(evaluator.evaluate("browser == \"firefox\" OR date < \"2030-01-01\"", req));
    
    // Оба false
    EXPECT_FALSE(evaluator.evaluate("browser == \"firefox\" OR date < \"2020-01-01\"", req));
}

// Тест: скобки изменяют приоритет
TEST(DSLEvaluatorTest, ParenthesesPrecedence)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 Chrome/120.0.0.0"}}};
    
    // (false OR true) AND true = true
    EXPECT_TRUE(evaluator.evaluate("(browser == \"firefox\" OR browser == \"chrome\") AND date < \"2030-01-01\"", req));
    
    // false OR (true AND false) = false
    EXPECT_FALSE(evaluator.evaluate("browser == \"firefox\" OR (browser == \"chrome\" AND date < \"2020-01-01\")", req));
}

// Тест: переменная country
TEST(DSLEvaluatorTest, CountryVariable)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    
    // Захардкожено RU
    EXPECT_TRUE(evaluator.evaluate("country == \"RU\"", req));
    EXPECT_FALSE(evaluator.evaluate("country == \"US\"", req));
}

// Тест: операторы сравнения
TEST(DSLEvaluatorTest, ComparisonOperators)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    
    EXPECT_TRUE(evaluator.evaluate("date <= \"2030-01-01\"", req));
    EXPECT_TRUE(evaluator.evaluate("date >= \"2020-01-01\"", req));
}

// Тест: кэширование условий
TEST(DSLEvaluatorTest, Caching)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req1{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    RedirectRequest req2{"test", "0.0.0.0", {{"User-Agent", "Firefox/121.0"}}};
    
    // Первый вызов - парсинг
    bool result1 = evaluator.evaluate("browser == \"chrome\"", req1);
    EXPECT_TRUE(result1);
    
    // Второй вызов того же условия - из кэша (но с другим запросом)
    bool result2 = evaluator.evaluate("browser == \"chrome\"", req2);
    EXPECT_FALSE(result2);
    
    // Третий вызов того же условия - из кэша
    bool result3 = evaluator.evaluate("browser == \"chrome\"", req1);
    EXPECT_TRUE(result3);
}

// Тест: определение разных браузеров
TEST(DSLEvaluatorTest, BrowserDetection)
{
    DSLEvaluator evaluator;
    
    RedirectRequest chrome{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"}}};
    RedirectRequest firefox{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:121.0) Gecko/20100101 Firefox/121.0"}}};
    RedirectRequest safari{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15"}}};
    RedirectRequest edge{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0"}}};
    
    EXPECT_TRUE(evaluator.evaluate("browser == \"chrome\"", chrome));
    EXPECT_TRUE(evaluator.evaluate("browser == \"firefox\"", firefox));
    EXPECT_TRUE(evaluator.evaluate("browser == \"safari\"", safari));
    EXPECT_TRUE(evaluator.evaluate("browser == \"edge\"", edge));
}

// Тест: сложное выражение
TEST(DSLEvaluatorTest, ComplexExpression)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Mozilla/5.0 Chrome/120.0"}}};
    
    bool result = evaluator.evaluate(
        "(browser == \"chrome\" OR browser == \"firefox\") AND "
        "(country == \"RU\" OR country == \"US\") AND "
        "date < \"2030-01-01\"",
        req
    );
    
    EXPECT_TRUE(result);
}

// Тест: некорректное условие
TEST(DSLEvaluatorTest, InvalidCondition)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    
    // Синтаксическая ошибка - должно вернуть false
    EXPECT_FALSE(evaluator.evaluate("browser = \"chrome\"", req));
    EXPECT_FALSE(evaluator.evaluate("browser ==", req));
    EXPECT_FALSE(evaluator.evaluate("== \"chrome\"", req));
}

// Тест: пустое условие
TEST(DSLEvaluatorTest, EmptyCondition)
{
    DSLEvaluator evaluator;
    
    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};
    
    EXPECT_FALSE(evaluator.evaluate("", req));
}

// Тест: неизвестная переменная возвращает false
TEST(DSLEvaluatorTest, UnknownVariable) {
    DSLEvaluator evaluator;

    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};

    // Переменной "os" нет в getVariableValue -> false
    EXPECT_FALSE(evaluator.evaluate("os == \"windows\"", req));

    // Любое сравнение с неизвестной переменной
    EXPECT_FALSE(evaluator.evaluate("unknownVar != \"something\"", req));
}

// Тест: некорректный синтаксис должен вернуть false
TEST(DSLEvaluatorTest, MalformedExpressionReturnsFalse) {
    DSLEvaluator evaluator;

    RedirectRequest req{"test", "0.0.0.0", {{"User-Agent", "Chrome/120.0"}}};

    EXPECT_FALSE(evaluator.evaluate("browser === \"chrome\"", req)); // тройное ==
    EXPECT_FALSE(evaluator.evaluate("AND browser == \"chrome\"", req)); // начинается с AND
    EXPECT_FALSE(evaluator.evaluate("browser == \"chrome\" OR", req)); // заканчивается OR
}
