#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "LogCommand.hpp"
#include "TestCommands.hpp"

/**
 * @brief ����� ��� LogCommand
 */
class LogCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ������� �������� ���� ���� ����� ������ ������
        testLogFile_ = "test.log";
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    void TearDown() override {
        // ������� �������� ���� ����� �����
        if (std::filesystem::exists(testLogFile_)) {
            std::filesystem::remove(testLogFile_);
        }
    }

    std::string testLogFile_;
};

/**
 * @brief ���� �������� LogCommand � ����������
 */
TEST_F(LogCommandTest, CreateWithMessage) {
    std::string message = "Test log message";
    LogCommand cmd(message, testLogFile_);

    EXPECT_EQ(cmd.getMessage(), message);
}

/**
 * @brief ���� ���������� LogCommand
 */
TEST_F(LogCommandTest, ExecuteWritesToFile) {
    std::string message = "Test log entry";
    LogCommand cmd(message, testLogFile_);

    // ��������� �������
    ASSERT_NO_THROW(cmd.execute());

    // ���������, ��� ���� ������
    EXPECT_TRUE(std::filesystem::exists(testLogFile_));

    // ��������� ���������� �����
    std::ifstream file(testLogFile_);
    std::string line;
    std::getline(file, line);

    // ���������, ��� ��������� ������������ � ����
    EXPECT_NE(line.find(message), std::string::npos);
}

/**
 * @brief ���� �������� LogCommand � �������� � �����������
 */
TEST_F(LogCommandTest, CreateWithCommandAndException) {
    auto failCmd = std::make_shared<FailCommand>("TestFail", "Test error");
    std::runtime_error error("Test exception");

    LogCommand cmd(failCmd, error, testLogFile_);

    // ���������, ��� ��������� �������� ���������� �� ������
    EXPECT_NE(cmd.getMessage().find("Test exception"), std::string::npos);
}

/**
 * @brief ���� ������������� ������� � ���
 */
TEST_F(LogCommandTest, MultipleLogEntries) {
    LogCommand cmd1("First entry", testLogFile_);
    LogCommand cmd2("Second entry", testLogFile_);

    cmd1.execute();
    cmd2.execute();

    // ���������, ��� ���� �������� ��� ������
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
 * @brief ���� ��������� null �������
 */
TEST_F(LogCommandTest, HandleNullCommand) {
    std::runtime_error error("Test error");

    // �������� � null �������� �� ������ �������� ����������
    ASSERT_NO_THROW({
        LogCommand cmd(nullptr, error, testLogFile_);
        cmd.execute();
        });
}