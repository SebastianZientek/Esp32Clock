#pragma once

#include "DisplayDriver.hpp"
#include <memory>
#include <functional>
#include <array>

class ClockDisplay
{
public:
    ClockDisplay(std::shared_ptr<DisplayDriver> displayDriver);
    void init();
    void printTime(int hour, int minute, int second, bool big = true);

private:
    std::array<std::function<void(int pos)>, 10> m_numbers;
    std::shared_ptr<DisplayDriver> m_displayDriver;

    void prepareFonts(int variant);
    void prepareNumbers();
    void printTimeBig(int hour, int minute, int second);
    void printTimeAndDate(int hour, int minute, int second);
};
