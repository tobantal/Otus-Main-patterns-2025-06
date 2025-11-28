#include "RedirectServiceApp.hpp"
#include <iostream>
#include <csignal>
#include <memory>

// Глобальный указатель для обработки сигналов
std::unique_ptr<RedirectServiceApp> g_app;

void signalHandler(int signal)
{
    std::cout << std::endl;
    std::cout << "[Main] Received signal " << signal << ", stopping server..." << std::endl;
    
    if (g_app)
    {
        g_app->stop();
    }
}

/**
 * @brief Точка входа в приложение
 */
int main(int argc, char* argv[])
{
    try
    {
        // Настраиваем обработчик сигналов
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        std::cout << "========================================" << std::endl;
        std::cout << "  Redirect Service - Microservice Demo " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        // Создаем приложение
        g_app = std::make_unique<RedirectServiceApp>();
        
        std::cout << "========================================" << std::endl;
        std::cout << "  Starting server..." << std::endl;
        std::cout << "  Press Ctrl+C to stop" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        // Запуск (блокирующий вызов) с передачей аргументов
        g_app->run(argc, argv);

        std::cout << "[Main] Application stopped" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Main] Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}