#pragma once

#include <Application.hpp>
#include "AuthController.hpp"
#include <IoC.hpp>
#include <iostream>
#include <RSAJWTGenerator.hpp>
#include "GameRegistry.hpp"
#include <memory>

class AuthApplication : public Application
{
protected:
    void configure() override
    {
        std::cout << "Configuring Auth Service..." << std::endl;

        // Создаём ОДИН Singleton
        static auto registry = std::make_shared<GameRegistry>();
        static auto jwtGen = std::make_shared<RSAJWTGenerator>("keys/private_key.pem");

        // Factory возвращает ТОТ ЖЕ объект
        auto registryFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            [registry]() -> std::shared_ptr<void>
            {                                                    // Захватываем registry!
                return std::static_pointer_cast<void>(registry); // Возвращаем ТОЖ ЖЕ!
            });

        auto registryKey = std::make_shared<std::string>("GameRegistry");
        std::vector<std::shared_ptr<void>> registryArgs = {registryKey, registryFactory};

        auto registerRegistry = IoC::resolve<ICommand>("IoC.Register", registryArgs);
        registerRegistry->execute();

        auto jwtFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            [jwtGen]() -> std::shared_ptr<void>
            {                                                  // ← Захватываем jwtGen!
                return std::static_pointer_cast<void>(jwtGen); // ← Возвращаем ТОЖ ЖЕ!
            });

        auto jwtKey = std::make_shared<std::string>("RSAJWTGenerator");
        std::vector<std::shared_ptr<void>> jwtArgs = {jwtKey, jwtFactory};

        auto registerJwt = IoC::resolve<ICommand>("IoC.Register", jwtArgs);
        registerJwt->execute();
    }

    void configureControllers(httplib::Server &server) override
    {
        auto registry = IoC::resolve<GameRegistry>("GameRegistry");
        auto jwtGen = IoC::resolve<RSAJWTGenerator>("RSAJWTGenerator");

        auto controller = std::make_shared<AuthController>(registry, jwtGen);

        controller->registerRoutes(server);
    }
};
