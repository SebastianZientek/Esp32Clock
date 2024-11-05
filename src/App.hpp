#pragma once

#include <memory>

#include "ClockDisplay.hpp"
#include "DisplayDriver.hpp"
#include "State.hpp"
#include "ButtonsManager.hpp"
#include "TimeManager.hpp"

class App
{
public:
    void init();
    void update();

private:
    std::shared_ptr<DisplayDriver> m_displayDriver{std::make_shared<DisplayDriver>()};
    
    std::shared_ptr<StateKeeper> m_stateKeeper;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<ClockDisplay> m_clockDisplay;
    ButtonsManager m_buttonsMgr;
};
