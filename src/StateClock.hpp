#pragma once

#include "App.hpp"
#include "State.hpp"

class StateClock : public State<App>
{
public:
    StateClock(std::shared_ptr<App> app)
        : State<App>(app) {};
    void process() override
    {
    }
    void shortClick() override
    {
    }
    void longClick() override
    {
    }
};
