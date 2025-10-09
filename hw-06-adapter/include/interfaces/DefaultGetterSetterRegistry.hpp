#pragma once

#include "IoC.hpp"
#include "ICommand.hpp"
#include "CommandGetter.hpp"
#include "CommandSetter.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

/// @brief Регистрирует в IoC контейнере стандартные команды getter/setter для игровых объектов
class DefaultGetterSetterRegistry {
public:
    static void registerAll() {
        const std::vector<std::pair<std::string, std::vector<std::string>>> interfaces = {
            { "IMovable",   { "Position", "Velocity" } },
            { "IRotatable", { "Direction", "AngularVelocity" } }
        };

        for (auto const &iface : interfaces) {
            const std::string &ifaceName = iface.first;
            for (auto const &prop : iface.second) {
                const std::string getKey = ifaceName + ":" + prop + ".get";
                const std::string setKey = ifaceName + ":" + prop + ".set";

                // Регистрация фабрики для CommandGetter
                {
                    IocContainer::FactoryFunction getterFactory = [prop]() -> std::shared_ptr<void> {
                        // Возвращаем фабрику, которая принимает объект
                        return std::static_pointer_cast<void>(
                            std::make_shared<std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>)>>(
                                [prop](std::shared_ptr<IGameObject> obj) -> std::shared_ptr<ICommand> {
                                    return std::make_shared<CommandGetter>(obj, prop);
                                }
                            )
                        );
                    };
                    auto cmd = std::make_shared<RegisterCommand>(getKey, getterFactory);
                    cmd->execute();
                }

                // Регистрация фабрики для CommandSetter
                {
                    IocContainer::FactoryFunction setterFactory = [prop]() -> std::shared_ptr<void> {
                        // Возвращаем фабрику, которая принимает объект и создаёт CommandSetter без значения
                        return std::static_pointer_cast<void>(
                            std::make_shared<std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>)>>(
                                [prop](std::shared_ptr<IGameObject> obj) -> std::shared_ptr<ICommand> {
                                    return std::make_shared<CommandSetter>(obj, prop);
                                }
                            )
                        );
                    };
                    auto cmd = std::make_shared<RegisterCommand>(setKey, setterFactory);
                    cmd->execute();
                }
            }
        }
    }
};