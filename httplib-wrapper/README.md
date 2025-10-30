# httplib-wrapper

Обёртка (wrapper) для популярной C++ HTTP библиотеки **cpp-httplib**.

## Описание

**httplib-wrapper** — это header-only библиотека, которая предоставляет удобный интерфейс для работы с HTTP сервером и клиентом в C++17.

Используется во всех микросервисах архитектуры "Космический бой":
- Auth Service (ДЗ-10)
- Battle Service (ДЗ-10)
- Может быть расширена для других сервисов

## Структура

```
httplib-wrapper/
├── include/
│   └── httplib.h          # Header-only библиотека cpp-httplib
├── CMakeLists.txt         # CMake конфигурация
└── README.md              # Этот файл
```

### Использовать в коде

```cpp
#include <httplib.h>

// Создание HTTP сервера
httplib::Server server;

// Регистрация endpoint
server.Post("/api/hello", [](const httplib::Request& req, httplib::Response& res) {
    res.set_content("Hello, World!", "text/plain");
});

// Запуск сервера
server.listen("0.0.0.0", 8080);
```

## Использование: HTTP Сервер

### Базовый сервер

```cpp
#include <httplib.h>

int main() {
    httplib::Server server;
    
    server.Post("/api/users", [](const httplib::Request& req, httplib::Response& res) {
        // Получить JSON из body
        auto json_body = req.body;
        
        // Отправить ответ
        res.status = 201;
        res.set_content(R"({"status":"created"})", "application/json");
    });
    
    server.listen("0.0.0.0", 8080);
    return 0;
}
```

### GET запрос

```cpp
server.Get("/api/games/:id", [](const httplib::Request& req, httplib::Response& res) {
    auto id = req.path_params.at("id");
    
    res.set_content(R"({"gameId":")" + id + R"("})", "application/json");
});
```

### Проверка заголовков

```cpp
server.Post("/api/command", [](const httplib::Request& req, httplib::Response& res) {
    // Получить заголовок
    auto auth = req.get_header_value("Authorization", "");
    
    if (auth.empty()) {
        res.status = 401;
        res.set_content(R"({"error":"unauthorized"})", "application/json");
        return;
    }
    
    res.set_content(R"({"status":"ok"})", "application/json");
});
```

## Использование: HTTP Клиент

```cpp
#include <httplib.h>

int main() {
    httplib::Client client("localhost", 8080);
    
    // POST запрос
    auto res = client.Post("/api/games/create",
        R"({"organizerId":"user_123"})",
        "application/json");
    
    if (res && res->status == 201) {
        std::cout << "Success: " << res->body << std::endl;
    }
    
    return 0;
}
```

## API Примеры

### Для микросервиса авторизации

```cpp
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

server.Post("/api/auth/games/create", 
    [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            
            // Валидация
            if (!body.contains("organizerId")) {
                res.status = 400;
                res.set_content(json{{"error", "invalid_request"}}.dump(),
                    "application/json");
                return;
            }
            
            // Обработка
            auto gameId = createGame(body["organizerId"]);
            
            res.status = 201;
            res.set_content(json{
                {"gameId", gameId},
                {"status", "created"}
            }.dump(), "application/json");
            
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json{{"error", e.what()}}.dump(),
                "application/json");
        }
    });
```

### Для проверки JWT токена

```cpp
server.Post("/game/command", 
    [](const httplib::Request& req, httplib::Response& res) {
        // Извлечение JWT из заголовка
        auto auth = req.get_header_value("Authorization", "");
        
        if (auth.empty()) {
            res.status = 401;
            res.set_content(json{{"error", "unauthorized"}}.dump(),
                "application/json");
            return;
        }
        
        // Парсинг "Bearer <token>"
        std::string token = auth;
        if (token.substr(0, 7) == "Bearer ") {
            token = token.substr(7);
        }
        
        // Проверка JWT
        try {
            auto payload = jwtVerifier->verify(token);
            res.status = 200;
            res.set_content(json{{"status", "accepted"}}.dump(),
                "application/json");
        } catch (...) {
            res.status = 401;
            res.set_content(json{{"error", "invalid_token"}}.dump(),
                "application/json");
        }
    });
```

## Интеграция с другими библиотеками

### С nlohmann/json

```cpp
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

auto body = json::parse(req.body);
auto response = json{{"status", "ok"}};
res.set_content(response.dump(), "application/json");
```

### С IoC контейнером

```cpp
#include <httplib.h>
#include <IoC.hpp>

auto controller = IoC::resolve<MyController>("MyController");
controller->registerRoutes(server);
```

## Типичные ошибки

### ❌ Неправильно:
```cpp
#include "http_server/httplib.h"
```

### ✅ Правильно:
```cpp
#include <httplib.h>
```

### ❌ Неправильно: забыли подключить в CMake
```cmake
# Отсутствует:
target_link_libraries(my_target PUBLIC httplib)
```

### ✅ Правильно:
```cmake
add_subdirectory(../httplib-wrapper build/httplib)
target_link_libraries(my_target PUBLIC httplib)
```

## Версия

- **httplib-wrapper**: 0.26.0
- **cpp-httplib**: 0.26.0 (из GitHub yhirose/cpp-httplib)

## Лицензия

cpp-httplib использует MIT лицензию.

## Документация

- Официальная документация cpp-httplib: https://github.com/yhirose/cpp-httplib
- Примеры: https://github.com/yhirose/cpp-httplib/tree/master/example

## Использование в ДЗ

### hw-08 (Message Exchange)
```cpp
#include <httplib.h>

httplib::Server server;
server.Post("/game/command", handleGameCommand);
server.listen("0.0.0.0", 8090);
```

### hw-10 (Authorization with JWT)
```cpp
#include <httplib.h>

// Auth Service
server.Post("/api/auth/token", handleIssueToken);

// Battle Service
server.Post("/game/command", handleGameCommand);
```

## Вопросы и поддержка

При возникновении проблем:
1. Проверьте, что `add_subdirectory()` добавлен в CMakeLists.txt
2. Проверьте, что `target_link_libraries()` содержит `httplib`
3. Проверьте, что используете `#include <httplib.h>` (без кавычек)
4. Смотрите примеры в `/example` в официальном репо cpp-httplib

