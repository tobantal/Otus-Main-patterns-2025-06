# Microservice Redirect Service - Dockerfile
# Author: Anton Tobolkin
# Multistage build: build stage + runtime stage

# ============================================================================
# Build Stage - сборка проекта
# ============================================================================
FROM gcc:13 AS builder

# Устанавливаем зависимости для сборки
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Создаём рабочую директорию
WORKDIR /build

# Копируем весь проект
COPY . .

# Собираем проект
RUN cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17 && \
    cmake --build build --config Release -j$(nproc)

# ============================================================================
# Runtime Stage - минимальный образ с исполняемым файлом
# ============================================================================
FROM ubuntu:24.04

# Устанавливаем только runtime зависимости
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Создаём рабочую директорию
WORKDIR /app

# Копируем только исполняемый файл из build stage
COPY --from=builder /build/build/hw-14-final-project/redirect-service/redirect-service /app/redirect-service

# Копируем конфигурацию по умолчанию
COPY --from=builder /build/hw-14-final-project/redirect-service/config.json /app/config.json

# Открываем порт
EXPOSE 8080

# Запускаем сервис
CMD ["./redirect-service"]