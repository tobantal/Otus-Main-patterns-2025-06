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
        std::cout << "=== Rule Service Starting ===" << std::endl;
        
        // Создаем приложение
        RuleServiceApp app;
        g_app = &app;
        
        // Устанавливаем обработчики сигналов
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
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