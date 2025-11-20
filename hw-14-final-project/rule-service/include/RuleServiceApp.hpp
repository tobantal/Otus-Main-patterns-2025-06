#pragma once

#include "BoostBeastApplication.hpp"

class RuleServiceApp : public BoostBeastApplication
{
public:
    RuleServiceApp();
    ~RuleServiceApp() override;

    void configureInjection() override;
};