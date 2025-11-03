#pragma once

#include "ICommandBuilder.hpp"
#include "Commands.hpp"
#include <iostream>

/**
 * @brief Builder для MoveCommand
 */
class MoveCommandBuilder : public ICommandBuilder {
public:
    std::shared_ptr<ICommand> build(const json& params) override {
        int x = params.value("x", 0);
        int y = params.value("y", 0);
        
        std::cout << "[Builder] Creating MoveCommand: x=" << x << ", y=" << y << std::endl;
        
        return std::make_shared<MoveCommand>(x, y);
    }
};

/**
 * @brief Builder для AttackCommand
 */
class AttackCommandBuilder : public ICommandBuilder {
public:
    std::shared_ptr<ICommand> build(const json& params) override {
        std::string target = params.value("target", "");
        
        std::cout << "[Builder] Creating AttackCommand: target=" << target << std::endl;
        
        return std::make_shared<AttackCommand>(target);
    }
};

/**
 * @brief Builder для RotateCommand
 */
class RotateCommandBuilder : public ICommandBuilder {
public:
    std::shared_ptr<ICommand> build(const json& params) override {
        int angle = params.value("angle", 0);
        
        std::cout << "[Builder] Creating RotateCommand: angle=" << angle << std::endl;
        
        return std::make_shared<RotateCommand>(angle);
    }
};
