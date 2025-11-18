# Установка Docker на Pop!_OS 24.04 / Ubuntu 24.04

## Шаг 1: Удаление старых версий (если есть)

```bash
sudo apt remove docker docker-engine docker.io containerd runc
```

## Шаг 2: Установка зависимостей

```bash
sudo apt update
sudo apt install -y \
    ca-certificates \
    curl \
    gnupg \
    lsb-release
```

## Шаг 3: Добавление официального GPG ключа Docker

```bash
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg
```

## Шаг 4: Добавление репозитория Docker

```bash
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

## Шаг 5: Установка Docker Engine

```bash
sudo apt update
sudo apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

## Шаг 6: Проверка установки

```bash
sudo docker --version
```

**Ожидаемый вывод:**
```
Docker version 24.0.7, build ...
```

## Шаг 7: Добавление пользователя в группу docker (без sudo)

```bash
# Добавить текущего пользователя в группу docker
sudo usermod -aG docker $USER

# Применить изменения (перелогиниться)
newgrp docker
```

**Или перезагрузи систему:**
```bash
sudo reboot
```

## Шаг 8: Проверка без sudo

```bash
docker --version
docker ps
```

Если команды работают без `sudo` - всё ок!

---

## 🐳 Тест: Hello World контейнер

### Запуск hello-world

```bash
docker run hello-world
```

**Ожидаемый вывод:**
```
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
...
Hello from Docker!
This message shows that your installation appears to be working correctly.
...
```

### Проверка запущенных контейнеров

```bash
# Список запущенных контейнеров
docker ps

# Список всех контейнеров (включая остановленные)
docker ps -a
```

### Список образов

```bash
docker images
```

Должен быть образ `hello-world`.

### Удаление hello-world (опционально)

```bash
# Удалить контейнер
docker rm $(docker ps -aq -f ancestor=hello-world)

# Удалить образ
docker rmi hello-world
```

---

## ✅ Проверка Docker Compose

```bash
docker compose version
```

**Ожидаемый вывод:**
```
Docker Compose version v2.21.0
```

---

## 🚨 Troubleshooting

### Ошибка: "Cannot connect to the Docker daemon"

```bash
# Проверить, запущен ли Docker
sudo systemctl status docker

# Запустить Docker
sudo systemctl start docker

# Добавить в автозагрузку
sudo systemctl enable docker
```

### Ошибка: "permission denied"

```bash
# Убедись, что пользователь в группе docker
groups $USER

# Если нет группы docker, добавь:
sudo usermod -aG docker $USER
newgrp docker
```

### Очистка всего Docker (если что-то пошло не так)

```bash
# Остановить все контейнеры
docker stop $(docker ps -aq)

# Удалить все контейнеры
docker rm $(docker ps -aq)

# Удалить все образы
docker rmi $(docker images -q)

# Удалить все volumes
docker volume prune -f

# Удалить всё (полная очистка)
docker system prune -a --volumes -f
```

---

## 📝 Полезные команды Docker

```bash
# Список образов
docker images

# Список контейнеров (запущенные)
docker ps

# Список контейнеров (все)
docker ps -a

# Логи контейнера
docker logs <container_name>

# Остановить контейнер
docker stop <container_name>

# Удалить контейнер
docker rm <container_name>

# Удалить образ
docker rmi <image_name>

# Зайти в контейнер
docker exec -it <container_name> /bin/bash

# Статистика контейнеров
docker stats
```

---

## ✅ Готово!

Docker установлен и готов к работе. Теперь можно запускать redirect-service!