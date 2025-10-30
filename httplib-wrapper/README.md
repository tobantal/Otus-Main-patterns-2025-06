# httplib-wrapper

Header-only обёртка с **cpp-httplib** и **jwt-cpp**.

## Содержит

- **cpp-httplib** (v0.26.0) — HTTP сервер и клиент
- **jwt-cpp** (v0.9.4) — JWT токены (RS256, HMAC и др.)

## Структура

```
httplib-wrapper/include/
├── httplib.h
└── jwt-cpp/
    ├── jwt.h
    ├── base.h
    └── traits/
        └── nlohmann-json/
            ├── defaults.h
            └── traits.h
```

## Использование

### CMakeLists.txt

```cmake
add_subdirectory(../httplib-wrapper build/httplib)
target_link_libraries(my_target PUBLIC httplib)
```

### Код

```cpp
#include <httplib.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
```

## Примеры

### HTTP сервер с JWT

```cpp
#include <httplib.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

httplib::Server server;

// Генерация JWT
server.Post("/token", [](const httplib::Request& req, httplib::Response& res) {
    auto token = jwt::create<jwt::traits::nlohmann_json>()
        .set_payload({{"sub", "user_123"}})
        .sign(jwt::algorithm::rs256(publicKey, privateKey));
    
    res.set_content(token, "text/plain");
});

// Проверка JWT
server.Post("/protected", [](const httplib::Request& req, httplib::Response& res) {
    auto auth = req.get_header_value("Authorization", "");
    
    try {
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(auth.substr(7));
        jwt::verify<jwt::traits::nlohmann_json>()
            .allow_algorithm(jwt::algorithm::rs256(publicKey))
            .verify(decoded);
        
        res.set_content("OK", "text/plain");
    } catch (...) {
        res.status = 401;
    }
});

server.listen("0.0.0.0", 8080);
```

## Версии

- cpp-httplib: 0.26.0
- jwt-cpp: 0.9.4
