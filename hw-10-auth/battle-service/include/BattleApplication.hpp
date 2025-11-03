#pragma once

#include "Application.hpp"
#include "BattleController.hpp"
#include <IoC.hpp>
#include <RSAJWTVerifier.hpp>
#include <game_manager.hpp>
#include <iostream>
#include "CommandBuilders.hpp"
#include "ICommandBuilder.hpp"

class BattleApplication : public Application
{
protected:
    void configure() override
    {
        std::cout << "Configuring Battle Service..." << std::endl;

        // Регистрируем GameManager
        auto gameManagerFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void>
            {
                return std::static_pointer_cast<void>(std::make_shared<GameManager>());
            });

        auto gameManagerKey = std::make_shared<std::string>("GameManager");
        std::vector<std::shared_ptr<void>> gameManagerArgs = {gameManagerKey, gameManagerFactory};

        auto registerGameManager = IoC::resolve<ICommand>("IoC.Register", gameManagerArgs);
        registerGameManager->execute();

        // Регистрируем RSAJWTVerifier
        std::string publicKeyPath = "keys/public_key.pem";

        auto verifierFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            [publicKeyPath]() -> std::shared_ptr<void>
            {
                return std::static_pointer_cast<void>(
                    std::make_shared<RSAJWTVerifier>(publicKeyPath));
            });

        auto verifierKey = std::make_shared<std::string>("RSAJWTVerifier");
        std::vector<std::shared_ptr<void>> verifierArgs = {verifierKey, verifierFactory};

        auto registerVerifier = IoC::resolve<ICommand>("IoC.Register", verifierArgs);
        registerVerifier->execute();

        // Builder для Move команды
        auto moveBuilderFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void>
            {
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<ICommandBuilder>(
                        std::make_shared<MoveCommandBuilder>()));
            });

        auto moveKey = std::make_shared<std::string>("CommandBuilder.Move");
        std::vector<std::shared_ptr<void>> moveArgs = {moveKey, moveBuilderFactory};
        auto registerMove = IoC::resolve<ICommand>("IoC.Register", moveArgs);
        registerMove->execute();

        // Builder для Attack команды
        auto attackBuilderFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void>
            {
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<ICommandBuilder>(
                        std::make_shared<AttackCommandBuilder>()));
            });

        auto attackKey = std::make_shared<std::string>("CommandBuilder.Attack");
        std::vector<std::shared_ptr<void>> attackArgs = {attackKey, attackBuilderFactory};
        auto registerAttack = IoC::resolve<ICommand>("IoC.Register", attackArgs);
        registerAttack->execute();

        // Builder для Rotate команды
        auto rotateBuilderFactory = std::make_shared<std::function<std::shared_ptr<void>()>>(
            []() -> std::shared_ptr<void>
            {
                return std::static_pointer_cast<void>(
                    std::static_pointer_cast<ICommandBuilder>(
                        std::make_shared<RotateCommandBuilder>()));
            });

        auto rotateKey = std::make_shared<std::string>("CommandBuilder.Rotate");
        std::vector<std::shared_ptr<void>> rotateArgs = {rotateKey, rotateBuilderFactory};
        auto registerRotate = IoC::resolve<ICommand>("IoC.Register", rotateArgs);
        registerRotate->execute();
    }

    void configureControllers(httplib::Server &server) override
    {
        std::cout << "Configuring Battle Controllers..." << std::endl;

        auto gameManager = IoC::resolve<GameManager>("GameManager");
        auto jwtVerifier = IoC::resolve<RSAJWTVerifier>("RSAJWTVerifier");

        auto controller = std::make_shared<BattleController>(gameManager, jwtVerifier);
        controller->registerRoutes(server);
    }
};
