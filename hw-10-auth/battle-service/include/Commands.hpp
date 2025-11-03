#pragma once

#include "ICommand.hpp"
#include <string>
#include <iostream>

/**
 * @brief Команды-заглушки для данного проекта.
 * В реальном проекте нужно импортировать common библиотеку или собрать hw-04 как библиотеку.
 * Здесь досточтоно просто тестовых команд.
 */

/**
 * @brief Команда движения
 */
class MoveCommand : public ICommand {
private:
    int x_;
    int y_;

public:
    MoveCommand(int x, int y) : x_(x), y_(y) {}

    void execute() override {
        std::cout << "Executing MoveCommand: x=" << x_ << ", y=" << y_ << std::endl;
    }
};

/**
 * @brief Команда атаки
 */
class AttackCommand : public ICommand {
private:
    std::string target_;

public:
    AttackCommand(const std::string& target) : target_(target) {}

    void execute() override {
        std::cout << "Executing AttackCommand: target=" << target_ << std::endl;
    }
};

/**
 * @brief Команда поворота
 */
class RotateCommand : public ICommand {
private:
    int angle_;

public:
    RotateCommand(int angle) : angle_(angle) {}

    void execute() override {
        std::cout << "Executing RotateCommand: angle=" << angle_ << std::endl;
    }
};
