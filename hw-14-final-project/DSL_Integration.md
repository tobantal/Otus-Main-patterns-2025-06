# Интеграция DSL-парсера в Redirect Service

## 📋 Созданные файлы

### Заголовки (include/services/):
1. **ASTNode.hpp** - структура AST (дерево абстрактного синтаксиса)
2. **RuleParser.hpp** - парсер DSL-выражений
3. **DSLEvaluator.hpp** - интерпретатор с кэшированием

### Реализации (src/services/):
1. **ASTNode.cpp** - фабричные методы для создания узлов
2. **RuleParser.cpp** - токенизация и рекурсивный спуск
3. **DSLEvaluator.cpp** - вычисление AST + кэш

### Тесты (tests/):
1. **RuleParserTest.cpp** - тесты парсера (15 тестов)
2. **DSLEvaluatorTest.cpp** - тесты интерпретатора (14 тестов)

---

## 🔧 Шаги интеграции

### Шаг 1: Скопировать файлы

```bash
# Заголовки
cp ASTNode.hpp hw-14-final-project/redirect-service/include/services/
cp RuleParser.hpp hw-14-final-project/redirect-service/include/services/
cp DSLEvaluator.hpp hw-14-final-project/redirect-service/include/services/

# Реализации
cp ASTNode.cpp hw-14-final-project/redirect-service/src/services/
cp RuleParser.cpp hw-14-final-project/redirect-service/src/services/
cp DSLEvaluator.cpp hw-14-final-project/redirect-service/src/services/

# Тесты
cp RuleParserTest.cpp hw-14-final-project/redirect-service/tests/
cp DSLEvaluatorTest.cpp hw-14-final-project/redirect-service/tests/
```

### Шаг 2: Обновить tests/CMakeLists.txt

Добавить новые тесты в список:

```cmake
add_executable(redirect-service-test
    RouteMatcherTest.cpp
    RuleParserTest.cpp       # ← добавить
    DSLEvaluatorTest.cpp     # ← добавить
)
```

### Шаг 3: Заменить AlwaysTrueDSLEvaluator на DSLEvaluator

Открыть `src/RedirectServiceApp.cpp` и изменить:

```cpp
// БЫЛО:
#include "adapters/AlwaysTrueDSLEvaluator.hpp"

auto injector = di::make_injector(
    di::bind<IRuleClient>().to<InMemoryRuleClient>().in(di::singleton),
    di::bind<IRuleEvaluator>().to<AlwaysTrueDSLEvaluator>().in(di::singleton),  // ← старое
    di::bind<IRedirectService>().to<RedirectService>().in(di::singleton)
);

// СТАЛО:
#include "services/DSLEvaluator.hpp"

auto injector = di::make_injector(
    di::bind<IRuleClient>().to<InMemoryRuleClient>().in(di::singleton),
    di::bind<IRuleEvaluator>().to<DSLEvaluator>().in(di::singleton),  // ← новое
    di::bind<IRedirectService>().to<RedirectService>().in(di::singleton)
);
```

### Шаг 4: Обновить тестовые правила с реальными DSL-условиями

Открыть `src/adapters/InMemoryRuleClient.cpp` и изменить:

```cpp
InMemoryRuleClient::InMemoryRuleClient()
{
    std::cout << "[InMemoryRuleClient] Initializing with test rules..." << std::endl;
    
    // Правило: работает только для Chrome
    rules_["promo"] = Rule{
        "promo",
        "https://example.com/promo",
        "browser == \"chrome\""  // ← реальное DSL-условие
    };
    
    // Правило: работает до 2026 года
    rules_["docs"] = Rule{
        "docs",
        "https://docs.example.com",
        "date < \"2026-01-01\""
    };
    
    // Правило: всегда активно
    rules_["blog"] = Rule{
        "blog",
        "https://blog.example.com",
        "country == \"RU\""
    };
    
    std::cout << "[InMemoryRuleClient] Loaded " << rules_.size() << " rules" << std::endl;
}
```

### Шаг 5: (Опционально) Удалить старые заглушки

После проверки работы можно удалить:
- `include/adapters/AlwaysTrueDSLEvaluator.hpp`
- `src/adapters/AlwaysTrueDSLEvaluator.cpp`

---

## ✅ Сборка и тестирование

### Сборка

```bash
cd ~/Documents/Otus-Education/Otus-Main-patterns-2025-06
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Запуск тестов

```bash
# Все тесты
cd build
ctest --output-on-failure

# Только тесты парсера
ctest -R RuleParser --output-on-failure

# Только тесты evaluator
ctest -R DSLEvaluator --output-on-failure
```

### Ожидаемые результаты

```
[==========] Running 29 tests from 3 test suites.
[----------] 7 tests from RouteMatcherTest
...
[----------] 15 tests from RuleParserTest
...
[----------] 14 tests from DSLEvaluatorTest
...
[==========] 29 tests from 3 test suites ran. (XXX ms total)
[  PASSED  ] 29 tests.
```

---

## 🚀 Проверка работы сервиса

### Запуск сервиса

```bash
cd build/hw-14-final-project/redirect-service
./redirect-service
```

### Тестирование с разными браузерами

```bash
# Chrome - должен пройти для /r/promo
curl -v -H "User-Agent: Mozilla/5.0 Chrome/120.0" http://localhost:8080/r/promo
# Ожидаем: HTTP 302, Location: https://example.com/promo

# Firefox - НЕ должен пройти для /r/promo
curl -v -H "User-Agent: Mozilla/5.0 Firefox/121.0" http://localhost:8080/r/promo
# Ожидаем: HTTP 404 или Condition not satisfied

# Любой браузер - должен пройти для /r/blog (country == "RU")
curl -v http://localhost:8080/r/blog
# Ожидаем: HTTP 302, Location: https://blog.example.com
```

---

## 📊 Coverage после интеграции

Запусти coverage:

```bash
cd build-coverage
ctest
gcovr --root .. \
      --filter '../hw-14-final-project/.*' \
      --exclude '.*/tests/.*' \
      --gcov-ignore-parse-errors=negative_hits.warn_once_per_file \
      --html-details coverage.html

xdg-open coverage.html
```

**Ожидаемое покрытие:**
- RuleParser: ~90%
- DSLEvaluator: ~90%
- Общее: значительно выше 8.7% 🎯

---

## 🐛 Troubleshooting

### Ошибка компиляции: "ASTNode.hpp not found"

Проверь, что файлы скопированы в правильные директории.

### Тесты падают: "Parse error"

Проверь синтаксис DSL-условий в InMemoryRuleClient.

### Сервис не применяет условия

Проверь, что в DI используется `DSLEvaluator`, а не `AlwaysTrueDSLEvaluator`.

---

## ✨ Поддерживаемый DSL синтаксис

### Переменные:
- `browser` - определяется из User-Agent (chrome, firefox, safari, edge)
- `date` - текущая дата в формате YYYY-MM-DD
- `country` - страна (пока захардкожено "RU")

### Операторы сравнения:
- `==` - равно
- `!=` - не равно
- `<` - меньше
- `>` - больше
- `<=` - меньше или равно
- `>=` - больше или равно

### Логические операторы:
- `AND` - конъюнкция (и)
- `OR` - дизъюнкция (или)
- `()` - скобки для группировки

### Примеры условий:

```
browser == "chrome"
date < "2026-01-01"
country == "RU"
browser == "chrome" AND date < "2026-01-01"
(country == "RU" OR country == "US") AND browser == "chrome"
(browser == "chrome" OR browser == "firefox") AND date < "2030-01-01"
```

---
