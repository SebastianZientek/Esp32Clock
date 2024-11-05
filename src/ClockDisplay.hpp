#pragma once

#include "DisplayDriver.hpp"
#include <memory>
#include <functional>
#include <array>
#include "TimeManager.hpp"

class ClockDisplay
{
public:
    ClockDisplay(std::shared_ptr<DisplayDriver> displayDriver, std::shared_ptr<TimeManager> timeManager);
    void init();
    void printTime(bool big = true);

private:
    std::array<std::function<void(int pos)>, 10> m_numbers;
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;

    void prepareFonts(int variant);
    void prepareNumbers();
    void printTimeBig();
    void printTimeAndDate();
};
