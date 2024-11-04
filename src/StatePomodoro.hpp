#pragma once

#include "State.hpp"

class StatePomodoro : public State
{
public:
    StatePomodoro(std::shared_ptr<StateKeeper> stateKeeper,
                  std::shared_ptr<DisplayDriver> displayDriver)
        : State(stateKeeper)
        , m_displayDriver(displayDriver)
    {
    }

    void process() override
    {
        m_displayDriver->setPos(0, 1);
        m_displayDriver->print("StatePomodoro");
    }

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
};
