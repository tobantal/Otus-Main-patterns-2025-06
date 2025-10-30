#include <gtest/gtest.h>
#include "JWTException.hpp"
#include "JWTPayload.hpp"
#include "RSAJWTGenerator.hpp"
#include "RSAJWTVerifier.hpp"
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

using json = nlohmann::json;

/**
 * @brief Базовый тест для JWT библиотеки
 */
class JWTLibTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::string testDir = std::string(__FILE__).substr(0, std::string(__FILE__).rfind('/'));
        privateKeyPath_ = testDir + "/keys/private_key.pem";
        publicKeyPath_ = testDir + "/keys/public_key.pem";
    }

    std::string privateKeyPath_;
    std::string publicKeyPath_;
};

// ============================================================================
// Тесты JWTPayload
// ============================================================================

TEST_F(JWTLibTest, JWTPayload_ToJson_CorrectStructure)
{
    JWTPayload payload;
    payload.sub = "user_123";
    payload.iss = "auth-service";
    payload.userId = "user_123";
    payload.gameId = "game_abc";
    payload.role = "participant";

    auto now = std::chrono::system_clock::now();
    payload.iat = now;
    payload.exp = now + std::chrono::hours(1);

    // Преобразуем в JSON
    json result = payload.toJson();

    // Проверяем структуру
    EXPECT_TRUE(result.contains("sub"));
    EXPECT_TRUE(result.contains("iss"));
    EXPECT_TRUE(result.contains("userId"));
    EXPECT_TRUE(result.contains("gameId"));
    EXPECT_TRUE(result.contains("role"));
    EXPECT_TRUE(result.contains("iat"));
    EXPECT_TRUE(result.contains("exp"));

    // Проверяем значения
    EXPECT_EQ(result["sub"], "user_123");
    EXPECT_EQ(result["iss"], "auth-service");
    EXPECT_EQ(result["userId"], "user_123");
    EXPECT_EQ(result["gameId"], "game_abc");
    EXPECT_EQ(result["role"], "participant");
}

TEST_F(JWTLibTest, JWTPayload_FromJson_CorrectParsing)
{
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto exp_time = std::chrono::system_clock::to_time_t(now + std::chrono::hours(1));

    json j = {
        {"sub", "user_456"},
        {"iss", "auth-service"},
        {"userId", "user_456"},
        {"gameId", "game_xyz"},
        {"role", "organizer"},
        {"iat", now_time},
        {"exp", exp_time}};

    // Парсим из JSON
    JWTPayload payload = JWTPayload::fromJson(j);

    // Проверяем значения
    EXPECT_EQ(payload.sub, "user_456");
    EXPECT_EQ(payload.iss, "auth-service");
    EXPECT_EQ(payload.userId, "user_456");
    EXPECT_EQ(payload.gameId, "game_xyz");
    EXPECT_EQ(payload.role, "organizer");
}

TEST_F(JWTLibTest, JWTPayload_IsExpired_NotExpired)
{
    JWTPayload payload;
    auto now = std::chrono::system_clock::now();
    payload.exp = now + std::chrono::hours(1); // Истечёт через час

    EXPECT_FALSE(payload.isExpired());
}

TEST_F(JWTLibTest, JWTPayload_IsExpired_Expired)
{
    JWTPayload payload;
    auto now = std::chrono::system_clock::now();
    payload.exp = now - std::chrono::hours(1); // Истёк час назад

    EXPECT_TRUE(payload.isExpired());
}

TEST_F(JWTLibTest, JWTPayload_RoundTrip_ConsistentData)
{
    JWTPayload original;
    original.sub = "user_789";
    original.iss = "auth-service";
    original.userId = "user_789";
    original.gameId = "game_123";
    original.role = "participant";

    auto now = std::chrono::system_clock::now();
    original.iat = now;
    original.exp = now + std::chrono::hours(2);

    // Преобразуем в JSON и обратно
    json j = original.toJson();
    JWTPayload recovered = JWTPayload::fromJson(j);

    // Проверяем что данные совпадают
    EXPECT_EQ(original.sub, recovered.sub);
    EXPECT_EQ(original.iss, recovered.iss);
    EXPECT_EQ(original.userId, recovered.userId);
    EXPECT_EQ(original.gameId, recovered.gameId);
    EXPECT_EQ(original.role, recovered.role);
}

// ============================================================================
// Тесты RSAJWTGenerator и RSAJWTVerifier
// ============================================================================

TEST_F(JWTLibTest, RSAJWTGenerator_LoadsPrivateKey)
{
    // Тест что приватный ключ загружается без ошибок
    EXPECT_NO_THROW({
        RSAJWTGenerator generator(privateKeyPath_);
    });
}

TEST_F(JWTLibTest, RSAJWTVerifier_LoadsPublicKey)
{
    // Тест что публичный ключ загружается без ошибок
    EXPECT_NO_THROW({
        RSAJWTVerifier verifier(publicKeyPath_);
    });
}

TEST_F(JWTLibTest, RSAJWTGenerator_InvalidKeyPath_ThrowsException)
{
    EXPECT_THROW({ RSAJWTGenerator generator("invalid/path/private_key.pem"); }, JWTException);
}

TEST_F(JWTLibTest, RSAJWTVerifier_InvalidKeyPath_ThrowsException)
{
    EXPECT_THROW({ RSAJWTVerifier verifier("invalid/path/public_key.pem"); }, JWTException);
}

TEST_F(JWTLibTest, RSAJWTGenerator_GenerateToken_ReturnsValidString)
{
    RSAJWTGenerator generator(privateKeyPath_);

    JWTPayload payload;
    payload.userId = "user_test";
    payload.gameId = "game_test";
    payload.role = "participant";

    std::string token = generator.generate(payload);

    // Проверяем что токен не пустой
    EXPECT_FALSE(token.empty());

    // Проверяем что токен содержит 3 части (header.payload.signature)
    size_t dotCount = std::count(token.begin(), token.end(), '.');
    EXPECT_EQ(dotCount, 2);
}

TEST_F(JWTLibTest, RSAJWTGenerator_GenerateToken_DifferentTokensForDifferentPayloads)
{
    RSAJWTGenerator generator(privateKeyPath_);

    JWTPayload payload1;
    payload1.userId = "user_1";
    payload1.gameId = "game_1";

    JWTPayload payload2;
    payload2.userId = "user_2";
    payload2.gameId = "game_2";

    std::string token1 = generator.generate(payload1);
    std::string token2 = generator.generate(payload2);

    // Разные payload должны генерировать разные токены
    EXPECT_NE(token1, token2);
}

// ============================================================================
// Тесты проверки JWT
// ============================================================================

TEST_F(JWTLibTest, RSAJWTVerifier_ValidToken_Success)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload original;
    original.userId = "user_verify";
    original.gameId = "game_verify";
    original.role = "participant";

    // Генерируем токен
    std::string token = generator.generate(original);

    // Проверяем токен
    EXPECT_NO_THROW({
        JWTPayload verified = verifier.verify(token);
        EXPECT_EQ(verified.userId, original.userId);
        EXPECT_EQ(verified.gameId, original.gameId);
        EXPECT_EQ(verified.role, original.role);
    });
}

TEST_F(JWTLibTest, RSAJWTVerifier_InvalidToken_ThrowsException)
{
    RSAJWTVerifier verifier(publicKeyPath_);

    std::string invalidToken = "invalid.token.here";

    EXPECT_THROW({ verifier.verify(invalidToken); }, JWTException);
}

TEST_F(JWTLibTest, RSAJWTVerifier_CorruptedToken_ThrowsException)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload payload;
    payload.userId = "user_corrupt";
    payload.gameId = "game_corrupt";

    std::string token = generator.generate(payload);

    // Портим токен (меняем последний символ подписи)
    token[token.length() - 1] = (token[token.length() - 1] == 'A') ? 'B' : 'A';

    EXPECT_THROW({ verifier.verify(token); }, JWTException);
}

// ============================================================================
// Тесты expiration
// ============================================================================

TEST_F(JWTLibTest, RSAJWTVerifier_ExpiredToken_ThrowsException)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload payload;
    payload.userId = "user_exp";
    payload.gameId = "game_exp";

    // Генерируем токен с очень коротким временем жизни (1 секунда)
    std::string token = generator.generate(payload, std::chrono::seconds(1));

    // Ждём истечения
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Пытаемся проверить истёкший токен
    EXPECT_THROW({ verifier.verify(token); }, JWTException);
}

TEST_F(JWTLibTest, RSAJWTVerifier_ValidTokenBeforeExpiration_Success)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload payload;
    payload.userId = "user_before_exp";
    payload.gameId = "game_before_exp";

    // Генерируем токен с временем жизни 10 секунд
    std::string token = generator.generate(payload, std::chrono::seconds(10));

    // Сразу проверяем (до истечения)
    EXPECT_NO_THROW({
        JWTPayload verified = verifier.verify(token);
        EXPECT_EQ(verified.userId, payload.userId);
    });
}

// ============================================================================
// Тесты isValid()
// ============================================================================

TEST_F(JWTLibTest, RSAJWTVerifier_IsValid_ValidToken_ReturnsTrue)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload payload;
    payload.userId = "user_valid";
    payload.gameId = "game_valid";

    std::string token = generator.generate(payload);

    EXPECT_TRUE(verifier.isValid(token));
}

TEST_F(JWTLibTest, RSAJWTVerifier_IsValid_InvalidToken_ReturnsFalse)
{
    RSAJWTVerifier verifier(publicKeyPath_);

    std::string invalidToken = "invalid.token.string";

    EXPECT_FALSE(verifier.isValid(invalidToken));
}

TEST_F(JWTLibTest, RSAJWTVerifier_IsValid_ExpiredToken_ReturnsFalse)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload payload;
    payload.userId = "user_isvalid_exp";
    payload.gameId = "game_isvalid_exp";

    std::string token = generator.generate(payload, std::chrono::seconds(1));
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_FALSE(verifier.isValid(token));
}

// ============================================================================
// Тесты payload extraction
// ============================================================================

TEST_F(JWTLibTest, RSAJWTVerifier_Verify_ExtractsPayloadCorrectly)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    JWTPayload original;
    original.userId = "alice";
    original.gameId = "game_001";
    original.role = "organizer";

    std::string token = generator.generate(original, std::chrono::hours(1));
    JWTPayload extracted = verifier.verify(token);

    EXPECT_EQ(extracted.userId, "alice");
    EXPECT_EQ(extracted.gameId, "game_001");
    EXPECT_EQ(extracted.role, "organizer");
    EXPECT_EQ(extracted.iss, "auth-service");
    EXPECT_EQ(extracted.sub, "alice");
}

// ============================================================================
// Интеграционные тесты
// ============================================================================

TEST_F(JWTLibTest, FullCycle_GenerateAndVerifyMultipleTokens)
{
    RSAJWTGenerator generator(privateKeyPath_);
    RSAJWTVerifier verifier(publicKeyPath_);

    // Генерируем 5 разных токенов
    std::vector<JWTPayload> payloads;
    std::vector<std::string> tokens;

    for (int i = 1; i <= 5; ++i)
    {
        JWTPayload p;
        p.userId = "user_" + std::to_string(i);
        p.gameId = "game_" + std::to_string(i);
        p.role = (i % 2 == 0) ? "organizer" : "participant";

        payloads.push_back(p);
        tokens.push_back(generator.generate(p));
    }

    // Проверяем каждый токен
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_NO_THROW({
            JWTPayload verified = verifier.verify(tokens[i]);
            EXPECT_EQ(verified.userId, payloads[i].userId);
            EXPECT_EQ(verified.gameId, payloads[i].gameId);
            EXPECT_EQ(verified.role, payloads[i].role);
        });
    }
}

TEST_F(JWTLibTest, CrossService_GeneratedTokensCanBeVerified)
{
    // Симулируем ситуацию когда один сервис генерирует, другой проверяет

    RSAJWTGenerator authServiceGenerator(privateKeyPath_);
    RSAJWTVerifier battleServiceVerifier(publicKeyPath_);

    JWTPayload gamePayload;
    gamePayload.userId = "gamer_123";
    gamePayload.gameId = "space_battle_001";
    gamePayload.role = "participant";

    // Auth Service генерирует токен
    std::string token = authServiceGenerator.generate(gamePayload, std::chrono::hours(2));

    // Battle Service проверяет токен
    EXPECT_NO_THROW({
        JWTPayload verified = battleServiceVerifier.verify(token);

        // Battle Service может проверить, что пользователь участвует в игре
        EXPECT_EQ(verified.gameId, "space_battle_001");
        EXPECT_EQ(verified.userId, "gamer_123");

        // Может проверить role
        EXPECT_EQ(verified.role, "participant");
    });
}
