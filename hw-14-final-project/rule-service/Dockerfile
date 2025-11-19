# Multistage build для rule-service
FROM ubuntu:22.04 AS builder

# Устанавливаем зависимости для сборки
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    pkg-config \
    libpq-dev \
    libpqxx-dev \
    && rm -rf /var/lib/apt/lists/*

# Копируем исходники
WORKDIR /app
COPY . .

# Собираем проект
WORKDIR /app/build
RUN cmake .. && cmake --build . --target rule-service

# Runtime stage
FROM ubuntu:22.04

# Устанавливаем runtime зависимости
RUN apt-get update && apt-get install -y \
    libpq5 \
    libpqxx-6.4 \
    && rm -rf /var/lib/apt/lists/*

# Копируем бинарник и конфиг
WORKDIR /app
COPY --from=builder /app/build/rule-service/rule-service .
COPY rule-service/config.json .

# Открываем порт
EXPOSE 8081

# Запускаем сервис
CMD ["./rule-service"]