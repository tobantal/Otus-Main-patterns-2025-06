#include "CheckCollisionCommand.hpp"
#include <stdexcept>

CheckCollisionCommand::CheckCollisionCommand(
    std::shared_ptr<IGameObject> object1,
    std::shared_ptr<IGameObject> object2,
    std::shared_ptr<ICollisionChecker> checker)
    : object1_(std::move(object1))
    , object2_(std::move(object2))
    , checker_(std::move(checker))
{
    if (!object1_ || !object2_) {
        throw std::invalid_argument("Objects cannot be null");
    }
    if (!checker_) {
        throw std::invalid_argument("Checker cannot be null");
    }
}

void CheckCollisionCommand::execute() {
    // Выполняем проверку коллизии
    // Результат можно использовать для дальнейших действий
    // (например, генерации события столкновения)
    checker_->checkCollision(*object1_, *object2_);
}

std::shared_ptr<IGameObject> CheckCollisionCommand::getObject1() const {
    return object1_;
}

std::shared_ptr<IGameObject> CheckCollisionCommand::getObject2() const {
    return object2_;
}