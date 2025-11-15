# 🚀 Microservices Project - Boost.DI + Boost.Beast Integration

Учебный проект демонстрирующий интеграцию Boost.DI и Boost.Beast в чистую микросервисную архитектуру.

**Автор:** Anton Tobolkin  
**Стек:** C++17, Boost.DI, Boost.Beast, Boost.Asio, CMake

---

## 📋 Структура проекта
```
microservices-project/
├── CMakeLists.txt                    # Корневой файл сборки (FetchContent для Boost)
│
├── microservice-core/                # 📚 Библиотека интерфейсов
│   ├── CMakeLists.txt
│   └── include/
│       ├── IWebApplication.hpp       # Template Method для lifecycle
│       ├── ServerConfig.hpp          # Конфигурация сервера
│       ├── IRequest.hpp              # Абстракция HTTP запроса
│       ├── IResponse.hpp             # Абстракция HTTP ответа
│       └── IHttpHandler.hpp          # Интерфейс обработчика запросов
│
├── microservice-boost/               # ⚙️ Библиотека реализации на Boost
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── BoostBeastApplication.hpp # Базовая реализация приложения
│   │   ├── BeastRequestAdapter.hpp   # Адаптер Boost.Beast -> IRequest
│   │   └── BeastResponseAdapter.hpp  # Адаптер Boost.Beast -> IResponse
│   └── src/
│       └── BoostBeastApplication.cpp
│
└── redirect-service/                 # 🎯 Конкретный микросервис
    ├── CMakeLists.txt
    ├── main.cpp                      # Точка входа
    ├── include/
    │   ├── RedirectServiceApp.hpp    # Главный класс приложения
    │   ├── handlers/
    │   │   └── GetUserHandler.hpp    # Хендлер GET /api/users
    │   ├── services/
    │   │   ├── IUserService.hpp      # Интерфейс бизнес-логики
    │   │   └── UserService.hpp       # Реализация
    │   └── repositories/
    │       ├── IUserRepository.hpp   # Интерфейс доступа к данным
    │       └── InMemoryUserRepository.hpp
    └── src/
        ├── RedirectServiceApp.cpp
        ├── handlers/
        │   └── GetUserHandler.cpp
        ├── services/
        │   └── UserService.cpp
        └── repositories/
            └── InMemoryUserRepository.cpp
```

---

## 🏗️ Архитектурные принципы

### Трехуровневая архитектура

1. **microservice-core** - Чистые интерфейсы
   - Не зависит ни от каких внешних библиотек
   - Определяет контракты для веб-приложений
   - Header-only библиотека

2. **microservice-boost** - Конкретная реализация на Boost
   - Реализует интерфейсы используя Boost.Beast и Boost.Asio
   - Предоставляет адаптеры (Adapter Pattern)
   - Изолирует детали реализации от бизнес-логики

3. **redirect-service** - Бизнес-логика
   - Использует только интерфейсы из microservice-core
   - Boost.DI для управления зависимостями
   - Не знает о деталях реализации HTTP сервера

### Применяемые паттерны

- **Template Method** - `IWebApplication::run()` определяет lifecycle: `configureInjection()` → `configureRoutes()` → `start()`
- **Dependency Injection** - Boost.DI для автоматической инъекции зависимостей
- **Repository Pattern** - абстракция доступа к данным (`IUserRepository`)
- **Adapter Pattern** - `BeastRequestAdapter`/`BeastResponseAdapter` абстрагируют от Boost.Beast
- **Strategy Pattern** - подключаемые хендлеры через `IHttpHandler`

---

## 🛠️ Требования

- **C++17** совместимый компилятор (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake** 3.14 или выше
- **Git** (для автоматического скачивания Boost)

### Автоматическая установка зависимостей

Проект использует **CMake FetchContent** для автоматического скачивания:
- **Boost 1.83.0** (Beast, Asio, System)
- **Boost.DI 1.3.0** (header-only IoC контейнер)

Ничего устанавливать вручную не нужно!

---

## 🚀 Сборка и запуск

### Быстрый старт
```bash
cd microservices-project
mkdir build && cd build
cmake ..
cmake --build .
./redirect-service/redirect-service
```

### Первая сборка

При первой сборке CMake скачает Boost (~5-10 минут), последующие сборки будут быстрыми.

Вывод при запуске:
```
========================================
  Redirect Service - Microservice Demo
========================================

[RedirectServiceApp] Application created

========================================
  Server will start on 0.0.0.0:8080
  Try: curl http://localhost:8080/api/users/1
  Press Ctrl+C to stop
========================================

[RedirectServiceApp] Configuring DI injector...
[RedirectServiceApp] DI injector configured, registered 1 handlers
[App] Starting HTTP server...
[Server] Listening on 0.0.0.0:8080
[Server] Server is ready to accept connections!
```

---

## 🧪 Тестирование

### Получить пользователя по ID
```bash
curl http://localhost:8080/api/users/1
```

**Ожидаемый ответ:**
```json
{"id": 1, "name": "Alice", "email": "alice@example.com"}
```

### Другие тестовые пользователи
```bash
curl http://localhost:8080/api/users/2  # Bob
curl http://localhost:8080/api/users/3  # Charlie
```

### Несуществующий пользователь
```bash
curl http://localhost:8080/api/users/999
```

**Ответ:**
```json
{"error": "User not found"}
```

### Несуществующий маршрут
```bash
curl http://localhost:8080/api/unknown
```

**Ответ:**
```
404 Not Found
```

---

## 📊 Логи работы

При запросе вы увидите подробные логи всех слоев архитектуры:
```
[Server] New connection accepted
[Session] Received request: GET /api/users/1
[RedirectServiceApp] Route key: GET:/api/users
[GetUserHandler] Handling request: GET /api/users/1
[GetUserHandler] Looking for user with id: 1
[Service] getUser called with id: 1
[Repository] Finding user by id: 1
[Repository] User found: Alice
[Service] User retrieved: Alice
[GetUserHandler] Returning user: {"id": 1, "name": "Alice", "email": "alice@example.com"}
[Session] Response sent with status: 200
```

---

## 🎯 Ключевые особенности реализации

### ✅ Boost.DI Integration
```cpp
// configureInjection() в RedirectServiceApp.cpp
auto injector = di::make_injector(
    di::bind<IUserRepository>().to<InMemoryUserRepository>().in(di::singleton),
    di::bind<IUserService>().to<UserService>().in(di::singleton)
);

// Автоматическая инъекция зависимостей
handlers_["GET:/api/users"] = injector.create<std::shared_ptr<GetUserHandler>>();
```

### ✅ Template Method Pattern
```cpp
// IWebApplication::run() - не виртуальный
void run() {
    configureInjection();  // Hook method
    configureRoutes();     // Hook method
    start();               // Hook method
}
```

### ✅ Handler-based Routing
```cpp
// Каждый эндпоинт - отдельный handler
class GetUserHandler : public IHttpHandler {
    void handle(IRequest& req, IResponse& res) override;
};

// Регистрация в map с ключом "Метод:Путь"
handlers_["GET:/api/users"] = handler;
```

### ✅ Adapter Pattern
```cpp
// Абстракция от Boost.Beast
BeastRequestAdapter req(beast_req);
BeastResponseAdapter res(beast_res);
handler->handle(req, res);
```

---

## 🔄 SOLID в действии

### Single Responsibility
- `GetUserHandler` - только обработка GET запросов
- `UserService` - только бизнес-логика пользователей
- `InMemoryUserRepository` - только доступ к данным

### Open/Closed
- Новые хендлеры добавляются без изменения существующего кода
- Новые реализации `IUserRepository` (например, MongoDB) подключаются через DI

### Liskov Substitution
- Любая реализация `IHttpHandler` работает в системе роутинга
- Любая реализация `IUserRepository` работает с `UserService`

### Interface Segregation
- Узкие интерфейсы: `IRequest`, `IResponse`, `IHttpHandler`
- Клиенты зависят только от нужных им методов

### Dependency Inversion
- Высокоуровневая логика (`UserService`) не зависит от деталей (`InMemoryUserRepository`)
- Зависимости через интерфейсы + Boost.DI

---

## 📦 Что демонстрирует проект

✅ **Чистая архитектура** - бизнес-логика изолирована от фреймворков  
✅ **Boost.DI** - настоящий IoC контейнер вместо самописного  
✅ **Boost.Beast** - production-ready HTTP сервер  
✅ **Adapter Pattern** - абстракция от библиотек  
✅ **Template Method** - управление lifecycle  
✅ **Handler-based routing** - масштабируемая маршрутизация  
✅ **SOLID принципы** - на каждом уровне архитектуры  

---

## 🔄 Следующие шаги

1. ✅ **Завершено:** Интеграция Boost.DI и Boost.Beast
2. ✅ **Завершено:** Template Method + Handler-based архитектура
3. 📋 **Планируется:**
   - Добавить POST/PUT/DELETE хендлеры
   - Асинхронность (Boost.Asio coroutines)
   - MongoDB адаптер вместо InMemory
   - Unit-тесты с Google Test
   - Docker Compose для развертывания

---

## 📝 Примечания

- Проект создан в учебных целях для демонстрации архитектурных паттернов
- Для продакшена требуется добавить: error handling, structured logging, metrics, health checks
- Boost.Beast поддерживает асинхронность - будет добавлено в следующих итерациях

---

## 👨‍💻 Автор

**Anton Tobolkin**  
Курс "Architecture and Design Patterns"

---

## 📚 Полезные ссылки

- [Boost.DI Documentation](https://boost-ext.github.io/di/)
- [Boost.Beast Documentation](https://www.boost.org/doc/libs/master/libs/beast/doc/html/index.html)
- [Clean Architecture by Robert Martin](https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html)