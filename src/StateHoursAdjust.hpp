#pragma once

#include <memory>

#include "DisplayDriver.hpp"
#include "State.hpp"
#include "StatePomodoro.hpp"
#include "StateBigClock.hpp"
#include "TimeManager.hpp"

class StateHoursAdjust : public State
{
public:
    StateHoursAdjust(std::shared_ptr<StateKeeper> stateKeeper,
                     std::shared_ptr<DisplayDriver> displayDriver,
                     std::shared_ptr<TimeManager> timeManager)
        : State(stateKeeper)
        , m_displayDriver(displayDriver)
        , m_timeManager(timeManager)
    {
        m_displayDriver->clear();
    }

    void process() override
    {
        m_displayDriver->setPos(0, 0);
        m_displayDriver->printf("Hours offset: %d", m_timeManager->getHoursOffset());

        m_displayDriver->setPos(0, 1);
        m_displayDriver->printf("Hour: %s", m_timeManager->getTimeString().c_str());
    }

    void onEvent(Event event) override
    {
        switch (event)
        {
        case Event::TIME_ADJUST:
            changeState<StateBigClock>(m_displayDriver, m_timeManager);
            break;
        case Event::ACCEPT:
            m_timeManager->increaseHoursOffset();
            m_displayDriver->clear();
            break;
        }
    }

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;
};
