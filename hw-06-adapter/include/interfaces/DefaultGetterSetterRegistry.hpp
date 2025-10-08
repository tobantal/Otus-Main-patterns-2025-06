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
class DefaultGetterSetterRegistry
{
public:
    /// @brief Зарегистрировать все дефолтные геттеры и сеттеры
    static void registerAll()
    {
        // Список интерфейсов и их свойств, для которых нужно зарегистрировать команды
        // При необходимости надо расширить этот список новыми интерфейсами и свойствами
        const std::vector<std::pair<std::string, std::vector<std::string>>> interfaces = {
            { "IMovable",   { "Position", "Velocity" } },
            { "IRotatable", { "Direction", "AngularVelocity" } }
        };

        for (auto const &iface : interfaces) {
            const std::string &ifaceName = iface.first;
            for (auto const &prop : iface.second) {
                // Формируем ключи
                const std::string getKey = ifaceName + ":" + prop + ".get";
                const std::string setKey = ifaceName + ":" + prop + ".set";

                // Регистрация фабрики для CommandGetter
                {
                    auto factory = std::make_shared<
                        std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>)>
                    >(
                        [prop](std::shared_ptr<IGameObject> obj) -> std::shared_ptr<ICommand> {
                            return std::make_shared<CommandGetter>(obj, prop);
                        }
                    );
                    std::vector<std::shared_ptr<void>> args = {
                        std::make_shared<std::string>(getKey),
                        std::make_shared<std::type_info const&>(typeid(ICommand)),
                        factory
                    };
                    auto cmd = IoC::resolve<ICommand>("IoC.Register", args);
                    cmd->execute();
                }

                // Регистрация фабрики для CommandSetter
                {
                    auto factory = std::make_shared<
                        std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>, std::any)>
                    >(
                        [prop](std::shared_ptr<IGameObject> obj, std::any value) -> std::shared_ptr<ICommand> {
                            return std::make_shared<CommandSetter>(obj, prop, value);
                        }
                    );
                    std::vector<std::shared_ptr<void>> args = {
                        std::make_shared<std::string>(setKey),
                        std::make_shared<std::type_info const&>(typeid(ICommand)),
                        factory
                    };
                    auto cmd = IoC::resolve<ICommand>("IoC.Register", args);
                    cmd->execute();
                }
            }
        }
    }
};
