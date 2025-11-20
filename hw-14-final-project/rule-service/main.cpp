#include "RuleServiceApp.hpp"
#include <iostream>
#include <csignal>

/**
 * @file main.cpp
 * @brief Точка входа для rule-service
 * @author Anton Tobolkin
 */

// Глобальный указатель для обработки сигналов
RuleServiceApp* g_app = nullptr;

void signalHandler(int signal)
{
    std::cout << "\n[main] Received signal " << signal << std::endl;
    if (g_app)
    {
        g_app->stop();
    }
}

int main(int argc, char* argv[])
{
    try
    { 
        // Создаем приложение
        RuleServiceApp app;
        g_app = &app;
        
        // Устанавливаем обработчики сигналов
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        std::cout << "========================================" << std::endl;
        std::cout << "  Starting server..." << std::endl;
        std::cout << "  Press Ctrl+C to stop" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        
        // Запускаем приложение
        app.run(argc, argv);
        
        std::cout << "=== Rule Service Stopped ===" << std::endl;
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[main] Fatal error: " << e.what() << std::endl;
        return 1;
    }
}