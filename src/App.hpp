#pragma once

#include <memory>

#include "ClockDisplay.hpp"
#include "DisplayDriver.hpp"
#include "State.hpp"
#include "ButtonsManager.hpp"

class App
{
public:
    void init();
    void update();

private:
    std::shared_ptr<DisplayDriver> m_displayDriver{std::make_shared<DisplayDriver>()};
    ClockDisplay m_clockDisplay{m_displayDriver};
    std::shared_ptr<StateKeeper> m_stateKeeper;
    ButtonsManager m_buttonsMgr;
};
