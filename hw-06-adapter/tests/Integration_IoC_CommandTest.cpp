#include <gtest/gtest.h>
#include <memory>
#include <any>
#include <string>
#include <unordered_map>

#include "IoC.hpp"
#include "IocContainer.hpp"
#include "Scope.hpp"
#include "DefaultGetterSetterRegistry.hpp"
#include "ICommand.hpp"
#include "CommandGetter.hpp"
#include "CommandSetter.hpp"
#include "IGameObject.hpp"
#include "Vector2D.hpp"

// Простая реализация IGameObject
class SimpleGameObject : public IGameObject {
public:
    void setProperty(const std::string& key, const std::any& value) override {
        props_[key] = value;
    }
    std::any getProperty(const std::string& key) const override {
        auto it = props_.find(key);
        if (it == props_.end()) throw std::runtime_error("no such prop");
        return it->second;
    }
private:
    std::unordered_map<std::string, std::any> props_;
};

TEST(IoCIntegration, RegisterAndResolveCommands) {
    // Создание скоупа
    auto createScopeCmd = IoC::resolve<ICommand>("Scopes.New", {
        std::make_shared<std::string>("test")
    });
    createScopeCmd->execute();
    auto setScopeCmd = IoC::resolve<ICommand>("Scopes.Current", {
        std::make_shared<std::string>("test")
    });
    setScopeCmd->execute();

    DefaultGetterSetterRegistry::registerAll();
    auto obj = std::make_shared<SimpleGameObject>();

    // Тест сеттера
    {
        auto setterFactory = IoC::resolve<
            std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>)>
        >("IMovable:Position.set");
        
        Vector2D pos{7, 14};
        auto setterCmd = (*setterFactory)(obj);
        auto setter = std::dynamic_pointer_cast<CommandSetter>(setterCmd);
        setter->setValue(pos);
        setter->execute();

        auto stored = std::any_cast<Vector2D>(obj->getProperty("Position"));
        EXPECT_EQ(stored.x, 7);
        EXPECT_EQ(stored.y, 14);
    }

    // Тест геттера
    {
        obj->setProperty("Position", Vector2D{21, 28});
        auto getterFactory = IoC::resolve<
            std::function<std::shared_ptr<ICommand>(std::shared_ptr<IGameObject>)>
        >("IMovable:Position.get");
        
        auto getterCmd = (*getterFactory)(obj);
        getterCmd->execute();
        auto result = std::dynamic_pointer_cast<CommandGetter>(getterCmd)->getResult();
        auto pos = std::any_cast<Vector2D>(result);
        EXPECT_EQ(pos.x, 21);
        EXPECT_EQ(pos.y, 28);
    }
}
