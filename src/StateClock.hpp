#pragma once

#include <memory>

#include "App.hpp"
#include "DisplayDriver.hpp"
#include "State.hpp"

class StateClock : public State
{
public:
    StateClock(std::shared_ptr<StateKeeper> stateKeeper,
               std::shared_ptr<DisplayDriver> displayDriver)
        : State(stateKeeper)
        , m_displayDriver(displayDriver)
    {
    }

    void process() override
    {
        m_displayDriver->setPos(0, 1);
        m_displayDriver->print("StateClock");
    }

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
};
