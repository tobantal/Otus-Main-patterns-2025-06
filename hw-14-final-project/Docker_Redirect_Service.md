# Запуск Redirect Service в Docker

## Предварительные требования

✅ Docker установлен (см. DOCKER_INSTALL.md)  
✅ Docker работает без sudo  
✅ Проверено: `docker run hello-world`

---

## 📁 Подготовка файлов

### 1. Структура проекта

Убедись, что структура такая:

```
Otus-Main-patterns-2025-06/
├── hw-14-final-project/
│   ├── docker-compose.yml           # ← должен быть здесь
│   ├── microservice-core/
│   ├── microservice-boost/
│   └── redirect-service/
│       ├── Dockerfile               # ← должен быть здесь
│       ├── config.json              # ← должен быть здесь
│       ├── include/
│       ├── src/
│       └── main.cpp
└── CMakeLists.txt
```

### 2. Скопируй файлы

```bash
# Из директории с загруженными файлами
cd ~/Downloads  # или где у тебя файлы

# Копируй Dockerfile
cp Dockerfile ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project/redirect-service/

# Копируй docker-compose.yml
cp docker-compose.yml ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project/
```

---

## 🚀 Запуск Redirect Service

### Шаг 1: Перейди в директорию проекта

```bash
cd ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project
```

### Шаг 2: Запусти сборку и старт

```bash
docker compose up --build
```

**Что происходит:**
1. Docker скачивает базовый образ `gcc:13` (~2GB, займет время при первом запуске)
2. Копирует весь проект в контейнер
3. Устанавливает CMake и зависимости
4. Скачивает Boost через FetchContent (~3-5 минут)
5. Компилирует проект
6. Создает runtime образ с исполняемым файлом
7. Запускает контейнер

**Ожидаемое время первой сборки:** 5-10 минут

### Шаг 3: Наблюдай за логами

Ты увидишь логи сборки:
```
[+] Building ...
...
=> [builder 1/4] FROM docker.io/library/gcc:13
=> [builder 2/4] RUN apt-get update && apt-get install
=> [builder 3/4] COPY . .
=> [builder 4/4] RUN cmake -B build ...
...
```

Затем логи запуска сервиса:
```
redirect-service  | [RedirectServiceApp] Loading environment from config...
redirect-service  | [RedirectServiceApp] Environment loaded:
redirect-service  | [RedirectServiceApp] Configuring DI injector...
redirect-service  | [Server] Starting server on 0.0.0.0:8080
redirect-service  | [Server] Server is ready to accept connections!
```

---

## ✅ Проверка работы

### В другом терминале:

```bash
# Тест 1: Redirect на promo
curl -v http://localhost:8080/r/promo

# Ожидаемый результат:
# HTTP/1.1 302 Found
# Location: https://example.com/promo

# Тест 2: Redirect на docs
curl -v http://localhost:8080/r/docs

# Тест 3: Несуществующее правило (404)
curl -v http://localhost:8080/r/unknown

# Ожидаемый результат:
# HTTP/1.1 404 Not Found
```

### Проверка из браузера:

Открой в браузере: http://localhost:8080/r/promo

Должен произойти редирект на https://example.com/promo

---

## 🛑 Остановка сервиса

### Способ 1: Ctrl+C в терминале

Нажми `Ctrl+C` в терминале где запущен `docker compose up`

### Способ 2: Остановка из другого терминала

```bash
cd ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project
docker compose down
```

---

## 🔄 Режимы запуска

### Фоновый режим (detached)

```bash
docker compose up -d --build
```

Сервис запустится в фоне.

**Просмотр логов:**
```bash
docker compose logs -f redirect-service
```

**Остановка:**
```bash
docker compose down
```

### Пересборка после изменений кода

```bash
docker compose up --build
```

### Перезапуск без пересборки

```bash
docker compose restart redirect-service
```

---

## 🔍 Отладка

### Проверка, что контейнер запущен

```bash
docker ps
```

Должен быть `redirect-service` с статусом `Up`.

### Просмотр логов

```bash
docker compose logs redirect-service
```

### Зайти внутрь контейнера

```bash
docker exec -it redirect-service /bin/bash
```

Внутри контейнера:
```bash
ls /app
cat /app/config.json
./redirect-service  # ручной запуск
```

### Проверка сети

```bash
docker network ls
docker network inspect hw-14-final-project_microservices-network
```

---

## ⚙️ Изменение конфигурации

### Редактирование config.json

```bash
# Отредактируй конфиг
nano ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project/redirect-service/config.json

# Перезапусти контейнер (не нужна пересборка!)
cd ~/Documents/Otus-Education/Otus-Main-patterns-2025-06/hw-14-final-project
docker compose restart redirect-service
```

Конфиг монтируется как volume, поэтому изменения применяются сразу после перезапуска.

---

## 🧹 Очистка

### Удалить контейнеры и сети

```bash
docker compose down
```

### Удалить образы

```bash
docker compose down --rmi all
```

### Полная очистка (включая volumes)

```bash
docker compose down --rmi all --volumes
```

---

## 🚨 Частые проблемы

### Ошибка: "port is already allocated"

Порт 8080 уже занят. Проверь:
```bash
sudo lsof -i :8080
# Или
sudo netstat -tulpn | grep 8080
```

Останови процесс или измени порт в docker-compose.yml:
```yaml
ports:
  - "8081:8080"  # Внешний порт 8081
```

### Ошибка при сборке: "failed to fetch Boost"

Проблема с интернетом или GitHub. Попробуй снова:
```bash
docker compose build --no-cache
```

### Контейнер падает сразу после запуска

Смотри логи:
```bash
docker compose logs redirect-service
```

Часто это проблемы с config.json или отсутствием прав.

---

## 📊 Статистика контейнера

```bash
# Использование ресурсов
docker stats redirect-service

# Размер образа
docker images | grep redirect-service
```

---

## ✅ Успешный запуск выглядит так:

```
$ docker compose up --build
[+] Building 300.5s (17/17) FINISHED
...
[+] Running 1/1
 ✔ Container redirect-service  Created
Attaching to redirect-service
redirect-service  | [RedirectServiceApp] Loading environment from config...
redirect-service  | [RedirectServiceApp]   server.host: 0.0.0.0
redirect-service  | [RedirectServiceApp]   server.port: 8080
redirect-service  | [InMemoryRuleClient] Loaded 3 rules
redirect-service  | [Server] Starting server on 0.0.0.0:8080
redirect-service  | [Server] Server is ready to accept connections!
```

**В другом терминале:**
```
$ curl -v http://localhost:8080/r/promo
...
< HTTP/1.1 302 Found
< Location: https://example.com/promo
...
```

---

## 🎉 Готово!

Redirect Service работает в Docker! 🚀