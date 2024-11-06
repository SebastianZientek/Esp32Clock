#pragma once

#include <Preferences.h>

#include <functional>
#include <memory>

#include "DisplayDriver.hpp"
#include "Event.hpp"
#include "State.hpp"
#include "TimeManager.hpp"

class StateBigClock : public State
{
public:
    StateBigClock(std::shared_ptr<StateKeeper> stateKeeper,
                  std::shared_ptr<DisplayDriver> displayDriver,
                  std::shared_ptr<TimeManager> timeManager);

    void process() override;
    void onEvent(Event event) override;

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;
    bool m_blinkDots = true;
    Preferences m_preferences;

    std::array<std::function<void(int pos)>, 10> m_numbers;

    void prepareFonts(int variant);
    void prepareNumbers();
};
