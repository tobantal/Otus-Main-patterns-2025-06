# Multi-stage build для rule-service

# Stage 1: Builder
FROM ubuntu:22.04 as builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libpq-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

WORKDIR /app/build
RUN cmake .. && cmake --build . --target rule-service

# Stage 2: Runtime
FROM ubuntu:22.04 as stage-1

RUN apt-get update && apt-get install -y \
    libpq5 \
    libpqxx-6.4 \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/rule-service/rule-service ./rule-service
COPY hw-14-final-project/rule-service/config.json .

# Открываем порт
EXPOSE 8081

# Запускаем приложение
CMD ["./rule-service"]
