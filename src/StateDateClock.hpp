#pragma once

#include <memory>

#include "DisplayDriver.hpp"
#include "State.hpp"
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

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;
};
