#pragma once

#include "BoostBeastApplication.hpp"

/**
 * @file RedirectServiceApp.hpp
 * @brief Главное приложение redirect-service
 * @author Anton Tobolkin
 */

/**
 * @class RedirectServiceApp
 * @brief Веб-приложение для обработки редиректов
 */
class RedirectServiceApp : public BoostBeastApplication
{
public:
    RedirectServiceApp();
    ~RedirectServiceApp() override;

    void configureInjection() override;
};