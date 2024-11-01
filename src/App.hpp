#pragma once

#include <memory>

#include "ClockDisplay.hpp"
#include "DisplayDriver.hpp"
#include "State.hpp"

class App : public std::enable_shared_from_this<App>
{
public:
    void init();
    void update();

private:
    std::shared_ptr<DisplayDriver> m_displayDriver{std::make_shared<DisplayDriver>()};
    ClockDisplay m_clockDisplay{m_displayDriver};
    std::shared_ptr<State<App>> m_state;
};
