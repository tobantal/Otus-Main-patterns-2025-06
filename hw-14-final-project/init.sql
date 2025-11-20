-- Подключаемся к БД
\connect redirect_db

-- Инициализация БД для rule-service
CREATE TABLE IF NOT EXISTS rules (
    id SERIAL PRIMARY KEY,
    short_id VARCHAR(255) UNIQUE NOT NULL,
    target_url TEXT NOT NULL,
    condition TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_rules_short_id ON rules(short_id);

-- Вставляем тестовые правила
INSERT INTO rules (short_id, target_url, condition) VALUES
    ('promo', 'https://example.com/promo', 'country == "RU"'),
    ('docs', 'https://docs.example.com', 'browser == "chrome" AND country == "US"'),
    ('blog', 'https://blog.example.com', 'device == "mobile" OR device == "tablet"')
ON CONFLICT (short_id) DO NOTHING;
