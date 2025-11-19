-- Инициализация БД для redirect-service
-- Author: Anton Tobolkin

CREATE TABLE IF NOT EXISTS rules (
    short_id VARCHAR(255) PRIMARY KEY,
    target_url TEXT NOT NULL,
    condition TEXT NOT NULL DEFAULT '',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Индекс для быстрого поиска
CREATE INDEX IF NOT EXISTS idx_rules_short_id ON rules(short_id);

-- Триггер для автоматического обновления updated_at
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_rules_updated_at
BEFORE UPDATE ON rules
FOR EACH ROW
EXECUTE FUNCTION update_updated_at_column();

-- Тестовые данные
INSERT INTO rules (short_id, target_url, condition) VALUES
    ('promo', 'https://example.com/promo', 'browser == "chrome"'),
    ('blog', 'https://blog.example.com', '')
ON CONFLICT (short_id) DO NOTHING;

-- Вывод результатов
SELECT COUNT(*) as rule_count FROM rules;