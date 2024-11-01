#include "ClockDisplay.hpp"

#include "DisplayDriver.hpp"
#include "Fonts.hpp"

ClockDisplay::ClockDisplay(std::shared_ptr<DisplayDriver> displayDriver)
    : m_displayDriver(displayDriver)
{
}

void ClockDisplay::init()
{
    prepareFonts(-1);
    prepareNumbers();
}

void ClockDisplay::printTime(int hour, int minute, int second, bool big)
{
    if (big)
    {
        printTimeBig(hour, minute, second);
    }
    else {
        printTimeAndDate(hour, minute, second);   
    }
}

void ClockDisplay::prepareFonts(int variant)
{
    m_displayDriver->createCustom(0, LT, variant);
    m_displayDriver->createCustom(1, UB, variant);
    m_displayDriver->createCustom(2, RT, variant);
    m_displayDriver->createCustom(3, LL, variant);
    m_displayDriver->createCustom(4, LB, variant);
    m_displayDriver->createCustom(5, LR, variant);
    m_displayDriver->createCustom(6, UMB, variant);
    m_displayDriver->createCustom(7, LMB, variant);
}

void ClockDisplay::prepareNumbers()
{
    m_numbers[0] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(0);
        m_displayDriver->printCustom(1);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(3);
        m_displayDriver->printCustom(4);
        m_displayDriver->printCustom(5);
    };

    m_numbers[1] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(1);
        m_displayDriver->printCustom(2);
        m_displayDriver->print(254);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->print(254);
        m_displayDriver->printCustom(3);
        m_displayDriver->print(254);
    };

    m_numbers[2] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(3);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(7);
    };

    m_numbers[3] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(5);
    };

    m_numbers[4] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(3);
        m_displayDriver->printCustom(4);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->print(254);
        m_displayDriver->print(254);
        m_displayDriver->printCustom(3);
    };

    m_numbers[5] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(0);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(6);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(5);
    };

    m_numbers[6] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(8);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(6);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(3);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(5);
    };

    m_numbers[7] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(1);
        m_displayDriver->printCustom(1);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->print(254);
        m_displayDriver->printCustom(8);
        m_displayDriver->print(254);
    };

    m_numbers[8] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(8);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printCustom(3);
        m_displayDriver->printCustom(7);
        m_displayDriver->printCustom(5);
    };

    m_numbers[9] = [this](int pos)
    {
        m_displayDriver->setPos(pos, 0);
        m_displayDriver->printCustom(8);
        m_displayDriver->printCustom(6);
        m_displayDriver->printCustom(2);
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->print(254);
        m_displayDriver->print(254);
        m_displayDriver->printCustom(3);
    };
}

void ClockDisplay::printTimeBig(int hour, int minute, int second)
{
    auto hFirst = hour / 10;
    auto hSecond = hour - hFirst * 10;

    auto mFirst = minute / 10;
    auto mSecond = minute - mFirst * 10;

    // Call function to print first number in given position
    m_numbers[hFirst](1);
    m_numbers[hSecond](5);

    m_numbers[mFirst](11);
    m_numbers[mSecond](15);
    
    m_displayDriver->setPos(9, 0);
    m_displayDriver->print(second % 2 ? '*' : ' ');
    m_displayDriver->setPos(9, 1);
    m_displayDriver->print(second % 2 ? '*' : ' ');
}

void ClockDisplay::printTimeAndDate(int hour, int minute, int second)
{
    auto currentPos = (hour * minute) % 13;
    static auto lastTimePos = currentPos;

    if (lastTimePos != currentPos)
    {
        m_displayDriver->clear();
        lastTimePos = currentPos;
    }

    m_displayDriver->setPos(lastTimePos, 0);
    m_displayDriver->printf("%02d:%02d:%02d", hour, minute, second);
}