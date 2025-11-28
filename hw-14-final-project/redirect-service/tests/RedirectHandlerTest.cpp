#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/RedirectHandler.hpp"
#include "domain/RedirectRequest.hpp"
#include "domain/RedirectResult.hpp"
#include "SimpleRequest.hpp"
#include "SimpleResponse.hpp"

using ::testing::_;
using ::testing::Return;

// Мок сервиса редиректа
class MockRedirectService : public IRedirectService {
public:
    MOCK_METHOD(RedirectResult, redirect, (const RedirectRequest&), (override));
};

TEST(RedirectHandlerTest, HandlesRedirect) {
    auto service = std::make_shared<MockRedirectService>();
    RedirectHandler handler(service);

    // создаём реальные объекты IRequest/IResponse
    SimpleRequest request("GET", "/r/abc123", "", "127.0.0.1", 8080, {{"User-Agent", "TestAgent"}});
    SimpleResponse response;

    // ожидаемый результат
    RedirectResult expectedResult;
    expectedResult.success = true;
    expectedResult.targetUrl = "http://redirected.com";
    expectedResult.errorMessage = "";

    // задаём поведение мока
    EXPECT_CALL(*service, redirect(_))
        .WillOnce(Return(expectedResult));

    // вызываем обработчик
    handler.handle(request, response);

    // проверяем, что ответ установлен корректно
    EXPECT_EQ(response.getStatus(), 302); // если RedirectHandler ставит 302 при success
    auto headers = response.getHeaders();
    EXPECT_EQ(headers["Location"], expectedResult.targetUrl);
    EXPECT_EQ(response.getBody(), ""); // если тело не устанавливается
}

TEST(RedirectHandlerTest, HandlesRedirectNotFound) {
    auto service = std::make_shared<MockRedirectService>();
    RedirectHandler handler(service);

    // создаём запрос
    SimpleRequest request("GET", "/r/unknown", "", "127.0.0.1", 8080, {{"User-Agent", "TestAgent"}});
    SimpleResponse response;

    // ожидаемый результат от сервиса редиректа
    RedirectResult expectedResult;
    expectedResult.success = false;
    expectedResult.targetUrl = "";
    expectedResult.errorMessage = "ShortId not found";

    // задаём поведение мока
    EXPECT_CALL(*service, redirect(_))
        .WillOnce(Return(expectedResult));

    // вызываем обработчик
    handler.handle(request, response);

    // проверяем, что статус 404
    EXPECT_EQ(response.getStatus(), 404);

    // тело ответа содержит сообщение об ошибке
    EXPECT_EQ(response.getBody(), "Not Found: " + expectedResult.errorMessage);

    // заголовки Location быть не должно
    auto headers = response.getHeaders();
    EXPECT_TRUE(headers.find("Location") == headers.end());
}
