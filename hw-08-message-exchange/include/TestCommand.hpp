// include/TestCommand.hpp
#pragma once

#include "ICommand.hpp"

class TestCommand : public ICommand {
public:
    TestCommand() = default;
    void execute() override {
        // Тестовая реализация
    }
};
