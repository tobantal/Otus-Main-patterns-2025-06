#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "LogCommand.hpp"
#include "TestCommands.hpp"

/**
 * @brief Тесты для LogCommand
 */
class LogCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Удаляем тестовый файл лога перед каждым тестом
        testLogFile_ = "test.log";
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    void TearDown() override {
        // Удаляем тестовый файл после теста
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    std::string testLogFile_;
};

/**
 * @brief Тест создания LogCommand с сообщением
 */
TEST_F(LogCommandTest, CreateWithMessage) {
    std::string message = "Test log message";
    LogCommand cmd(message, testLogFile_);

    EXPECT_EQ(cmd.getMessage(), message);
}

/**
 * @brief Тест выполнения LogCommand
 */
TEST_F(LogCommandTest, ExecuteWritesToFile) {
    std::string message = "Test log entry";
    LogCommand cmd(message, testLogFile_);

    // Выполняем команду
    ASSERT_NO_THROW(cmd.execute());

    // Проверяем, что файл создан
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));

    // Проверяем содержимое файла
    std::ifstream file(testLogFile_);
    std::string line;
    std::getline(file, line);

    // Проверяем, что сообщение присутствует в логе
    EXPECT_NE(line.find(message), std::string::npos);
}

/**
 * @brief Тест создания LogCommand с командой и исключением
 */
TEST_F(LogCommandTest, CreateWithCommandAndException) {
    auto failCmd = std::make_shared<FailCommand>("TestFail", "Test error");
    std::runtime_error error("Test exception");

    LogCommand cmd(failCmd, error, testLogFile_);

    // Проверяем, что сообщение содержит информацию об ошибке
    EXPECT_NE(cmd.getMessage().find("Test exception"), std::string::npos);
}

/**
 * @brief Тест множественных записей в лог
 */
TEST_F(LogCommandTest, MultipleLogEntries) {
    LogCommand cmd1("First entry", testLogFile_);
    LogCommand cmd2("Second entry", testLogFile_);

    cmd1.execute();
    cmd2.execute();

    // Проверяем, что файл содержит две строки
    std::ifstream file(testLogFile_);
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
        if (lineCount == 1) {
            EXPECT_NE(line.find("First entry"), std::string::npos);
        }
        else if (lineCount == 2) {
            EXPECT_NE(line.find("Second entry"), std::string::npos);
        }
    }

    EXPECT_EQ(lineCount, 2);
}

/**
 * @brief Тест обработки null команды
 */
TEST_F(LogCommandTest, HandleNullCommand) {
    std::runtime_error error("Test error");

    // Создание с null командой не должно вызывать исключение
    ASSERT_NO_THROW({
        LogCommand cmd(nullptr, error, testLogFile_);
        cmd.execute();
        });
}