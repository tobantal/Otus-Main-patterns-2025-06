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
    ('blog', 'https://blog.example.com', 'device == "mobile" OR device == "tablet"'),

    ('safari_ru', 'https://ru.example.com/safari',
        'browser == "safari" AND country == "RU"'),

    ('night_22', 'https://example.com/night22',
        'date == "2025-11-22"'),

    ('ip_internal', 'https://intranet.example.com',
        'ip == "10.0.0.1"'),

    ('holiday_fixed', 'https://holiday.example.com',
        'date == "2025-12-31"'),

    ('edge_de', 'https://de.example.com/edge',
        'browser == "edge" AND country == "DE"'),

    ('mobile_us', 'https://us.example.com/mobile',
        'device == "mobile" AND country == "US"'),

    ('china', 'https://cn.example.com',
        'country == "CN"'),

    ('chrome_fr_or_tablet', 'https://fr.example.com/alt',
        'browser == "chrome" AND country == "FR" OR device == "tablet"'),

    ('unknown_ua', 'https://fallback.example.com',
        'browser == "unknown"'),

    ('premium_app', 'https://premium.example.com',
        'header.User-Agent == "PremiumApp"')
        
ON CONFLICT (short_id) DO NOTHING;
