#include "MacroCommand.hpp"

MacroCommand::MacroCommand(const std::vector<std::shared_ptr<ICommand>>& commands)
    : m_commands(commands) {
    if (commands.empty()) {
        throw std::invalid_argument("Commands vector cannot be empty");
    }

    for (const auto& cmd : commands) {
        if (!cmd) {
            throw std::invalid_argument("Commands vector cannot contain null commands");
        }
    }
}

void MacroCommand::execute() {
    try {
        // Выполняем команды последовательно
        for (const auto& command : m_commands) {
            command->execute();
        }
    }
    catch (const CommandException& e) {
        // Пробрасываем CommandException как есть
        throw CommandException("MacroCommand failed: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        // Преобразуем другие исключения в runtime_error
        throw std::runtime_error("Failed to execute macro command: " + std::string(e.what()));
    }
}