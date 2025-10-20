#include "InterpretCommand.hpp"
#include "TestCommand.hpp"

InterpretCommand::InterpretCommand(GameManager& manager)
  : manager_(manager)
  , container_(IocContainer::getInstance())
{
    container_.registerGlobalDependency(
        "test",
        []() { return std::make_shared<TestCommand>(); }
    );
}

void InterpretCommand::execute() {
    // Получаем JSON из IoC
    auto msgPtr = container_.getCurrentScope()->resolve<nlohmann::json>("incomingMessage");
    auto& msg = *msgPtr;

    if (!msg.contains("gameId") || !msg.contains("operationId")) {
        throw std::runtime_error("invalid_message");
    }

    auto gameId = msg["gameId"].get<std::string>();
    auto opId   = msg["operationId"].get<std::string>();

    auto queue = manager_.getOrCreate(gameId);
    auto cmd   = container_.resolve<ICommand>(opId);
    queue->push(cmd);
}
