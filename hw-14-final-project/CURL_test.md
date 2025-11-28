# 1. Создать правило с нормальным условием
curl -X POST http://localhost:8081/rules \
  -H "Content-Type: application/json" \
  -d '{
    "shortId": "chrome-rule",
    "targetUrl": "https://chrome.example.com",
    "condition": "browser == \"chrome\""
  }'

# 2. Создать правило с AND
curl -X POST http://localhost:8081/rules \
  -H "Content-Type: application/json" \
  -d '{
    "shortId": "ru-chrome-rule",
    "targetUrl": "https://ru-chrome.example.com",
    "condition": "country == \"RU\" AND browser == \"chrome\""
  }'

# 3. Создать правило с OR
curl -X POST http://localhost:8081/rules \
  -H "Content-Type: application/json" \
  -d '{
    "shortId": "browsers-rule",
    "targetUrl": "https://browsers.example.com",
    "condition": "(browser == \"chrome\" OR browser == \"firefox\") AND date < \"2030-01-01\""
  }'

# 4. Получить из redirect-service (с кэшем)
curl -X GET http://localhost:8080/r/chrome-rule

# 5. Сбросить конкретное правило из кэша
curl -X DELETE http://localhost:8080/cache/invalidate/chrome-rule

# 6. Сбросить весь кэш
curl -X DELETE http://localhost:8080/cache/invalidate
