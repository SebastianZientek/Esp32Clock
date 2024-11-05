#pragma once

#include <memory>

#include "DisplayDriver.hpp"
#include "State.hpp"
#include "StatePomodoro.hpp"
#include "TimeManager.hpp"

class StateDateClock : public State
{
public:
    StateDateClock(std::shared_ptr<StateKeeper> stateKeeper,
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
        auto currentEpochMinute = m_timeManager->getEpochMinutes();
        static auto lastEpochMinute = currentEpochMinute;

        if (currentEpochMinute != std::exchange(lastEpochMinute, currentEpochMinute))
        {
            m_displayDriver->clear();
        }

        m_displayDriver->setPos(currentEpochMinute % 13, 0);
        m_displayDriver->print(m_timeManager->getTimeString().c_str());

        m_displayDriver->setPos(currentEpochMinute % 11, 1);
        m_displayDriver->print(m_timeManager->getDateString().c_str());
    }

    void onEvent(Event event) override
    {
        if (event == Event::POMODORO_CLOCK_SWITCH)
        {
            changeState<StatePomodoro>(m_displayDriver, m_timeManager);
        }
    }

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;
};
