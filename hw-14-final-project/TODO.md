# TODO: Redirect Service - Ближайшие задачи

## 🎯 Текущая цель: 90% code coverage

**Текущее состояние:** 8.7% lines coverage (47/540)  
**Target:** ≥90% lines coverage

---

## 📋 Задачи (без приоритета, все важные)

### 1. 🗑️ Удалить UserService и связанный код

**Что удалить:**
- [ ] `redirect-service/include/services/IUserService.hpp`
- [ ] `redirect-service/include/services/UserService.hpp`
- [ ] `redirect-service/src/services/UserService.cpp`
- [ ] `redirect-service/include/repositories/IUserRepository.hpp`
- [ ] `redirect-service/include/repositories/InMemoryUserRepository.hpp`
- [ ] `redirect-service/src/repositories/InMemoryUserRepository.cpp`
- [ ] `redirect-service/include/handlers/GetUserHandler.hpp`
- [ ] `redirect-service/src/handlers/GetUserHandler.cpp`
- [ ] Удалить из `RedirectServiceApp.cpp`:
  - Регистрацию в DI: `di::bind<IUserRepository>`, `di::bind<IUserService>`
  - Регистрацию хендлера: `handlers_["GET:/api/users/*"]`

**Причина:** UserService был только для "HelloWorld" демонстрации, не нужен для Redirect Service.

---

### 2. ✅ Написать unit-тесты для RedirectService

**Цель:** Покрыть бизнес-логику RedirectService тестами с моками.

**Что тестировать:**
- [ ] `RedirectService::redirect()` - основная логика
  - [ ] Тест: правило найдено, активно, условие выполнено → success
  - [ ] Тест: правило не найдено → error "Rule not found"
  - [ ] Тест: правило неактивно (isActive=false) → error "Rule is inactive"
  - [ ] Тест: условие DSL не выполнено → error "Condition not satisfied"
  - [ ] Тест: extractShortId корректно извлекает ID из пути

**Технологии:**
- GoogleTest + GoogleMock
- Моки для `IRuleClient` и `IRuleEvaluator`

**Файлы:**
- Создать: `redirect-service/tests/RedirectServiceTest.cpp`
- Обновить: `redirect-service/tests/CMakeLists.txt`

---

### 3. 🔌 Оставить заглушки (не трогать)

**Заглушки остаются без изменений:**
- ✅ `InMemoryRuleClient` - хардкод правил в конструкторе
- ✅ `AlwaysTrueDSLEvaluator` - всегда возвращает true

**Причина:** Нужны для интеграционных тестов и запуска микросервиса. Заменим позже на реальные реализации.

---

### 4. 🐳 Docker Compose - запуск микросервиса

**Задача:** Запустить `redirect-service` в Docker.

**Что сделать:**
- [ ] Создать `Dockerfile` для redirect-service
  - Multistage build (build stage + runtime stage)
  - Базовый образ: `ubuntu:24.04` или `gcc:13`
  - Копировать только исполняемый файл
- [ ] Создать `docker-compose.yml` в корне проекта
  - Сервис: `redirect-service`
  - Порт: `8080:8080`
  - Volume для конфига: `./redirect-service/config.json:/app/config.json`
- [ ] Проверить запуск:
  ```bash
  docker-compose up --build
  curl http://localhost:8080/r/promo
  ```

**Структура:**
```
.
├── docker-compose.yml
├── redirect-service/
│   ├── Dockerfile
│   └── config.json
```

---

### 5. 🧠 Реализовать DSL-парсер и RuleEvaluator

**Цель:** Заменить `AlwaysTrueDSLEvaluator` на реальный парсер DSL.

**DSL синтаксис (простой вариант):**
```
browser == "chrome"
date < "2026-01-01"
country == "RU"
browser == "chrome" AND date < "2026-01-01"
```

**Что реализовать:**
- [ ] Класс `DSLParser` - парсинг строки в AST (дерево выражений)
- [ ] Класс `DSLEvaluator : IRuleEvaluator` - вычисление AST
- [ ] Поддержка операторов:
  - [ ] `==` (равенство)
  - [ ] `!=` (неравенство)
  - [ ] `<`, `>`, `<=`, `>=` (сравнение)
  - [ ] `AND`, `OR` (логические)
- [ ] Поддержка переменных из `RedirectRequest`:
  - [ ] `browser` (User-Agent)
  - [ ] `date` (текущая дата)
  - [ ] `country` (опционально, можно захардкодить)

**Технологии:**
- Простой рекурсивный парсер (без PEGTL для начала)
- Или библиотека PEGTL (если нужна расширяемость)

**Файлы:**
- Создать: `redirect-service/include/services/DSLParser.hpp`
- Создать: `redirect-service/src/services/DSLParser.cpp`
- Создать: `redirect-service/include/adapters/DSLEvaluator.hpp`
- Создать: `redirect-service/src/adapters/DSLEvaluator.cpp`
- Обновить: `RedirectServiceApp.cpp` - заменить `AlwaysTrueDSLEvaluator` на `DSLEvaluator`

**Unit-тесты:**
- [ ] Тесты на парсер: корректные и некорректные выражения
- [ ] Тесты на evaluator: вычисление различных условий

---

## 📊 Прогресс к 90% coverage

| Компонент | Текущее покрытие | Цель |
|-----------|------------------|------|
| RouteMatcher | 100% ✅ | 100% |
| RedirectService | 0% | 90% |
| RedirectHandler | 0% | 90% |
| InMemoryRuleClient | 0% | 90% |
| DSLEvaluator (новый) | - | 90% |
| Environment | 0% | 80% |
| BoostBeastApplication | 0% | 70% (интеграционные тесты) |

---

## 🚀 Следующие шаги (после базовых задач)

- [ ] State Pattern - переключение конфигураций
- [ ] Command Pattern - управление правилами через AdminService
- [ ] MongoRuleRepository - реальное хранилище
- [ ] FileRuleRepository - альтернативное хранилище
- [ ] AdminService - REST API для управления
- [ ] AuthService на Boost.Beast - рефакторинг с cpp-httplib
- [ ] Интеграционные тесты (end-to-end)

---

## 📝 Примечания

- Порядок выполнения задач не критичен - они независимы
- Задачи 1-5 - это ближайшие цели на 1-2 дня
- Фокус на достижении 90% coverage через unit-тесты
- Docker нужен для демонстрации работы микросервиса
- DSL можно начать с простого парсера, расширить позже