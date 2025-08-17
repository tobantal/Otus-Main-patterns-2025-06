#include "MoveWithFuelCommand.hpp"
#include "CheckFuelCommand.hpp"
#include "MoveCommand.hpp"
#include "BurnFuelCommand.hpp"
#include "MovableAdapter.hpp"
#include "FuelableAdapter.hpp"

MoveWithFuelCommand::MoveWithFuelCommand(
    std::shared_ptr<IMovable> movable,
    std::shared_ptr<IFuelable> fuelable)
    : MacroCommand({
        std::make_shared<CheckFuelCommand>(fuelable),
        std::make_shared<MoveCommand>(movable),
        std::make_shared<BurnFuelCommand>(fuelable)
        }) {
    if (!movable) {
        throw std::invalid_argument("Movable object cannot be null");
    }
    if (!fuelable) {
        throw std::invalid_argument("Fuelable object cannot be null");
    }
}

std::shared_ptr<MoveWithFuelCommand> MoveWithFuelCommand::create(
    std::shared_ptr<IGameObject> gameObject) {
    auto movable = std::make_shared<MovableAdapter>(gameObject);
    auto fuelable = std::make_shared<FuelableAdapter>(gameObject);
    return std::make_shared<MoveWithFuelCommand>(movable, fuelable);
}