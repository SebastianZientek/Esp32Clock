#include "StateBigClock.hpp"

#include "Fonts.hpp"
#include "StateDateClock.hpp"
#include "StateHoursAdjust.hpp"

StateBigClock::StateBigClock(std::shared_ptr<StateKeeper> stateKeeper,
                             std::shared_ptr<DisplayDriver> displayDriver,
                             std::shared_ptr<TimeManager> timeManager)
    : State(stateKeeper)
    , m_displayDriver(displayDriver)
    , m_timeManager(timeManager)
{
    prepareFonts(-1);
    prepareNumbers();
    m_displayDriver->clear();

    m_preferences.begin("big_clock", true);
    m_blinkDots = m_preferences.getBool("blinkDots", true);
    m_preferences.end();
}

void StateBigClock::process()
{
    auto hour = m_timeManager->getHour();
    auto minute = m_timeManager->getMinute();
    auto second = m_timeManager->getSecond();

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
    m_displayDriver->print(second % 2 || !m_blinkDots ? '*' : ' ');
    m_displayDriver->setPos(9, 1);
    m_displayDriver->print(second % 2 || !m_blinkDots ? '*' : ' ');
}

void StateBigClock::onEvent(Event event)
{
    switch (event)
    {
    case Event::POMODORO_CLOCK_SWITCH:
        changeState<StateDateClock>(m_displayDriver, m_timeManager);
        break;
    case Event::TIME_ADJUST:
        changeState<StateHoursAdjust>(m_displayDriver, m_timeManager);
        break;
    case Event::POMODORO_PAUSE:
        m_blinkDots = !m_blinkDots;
        m_preferences.begin("big_clock", false);
        m_preferences.putBool("blinkDots", m_blinkDots);
        m_preferences.end();
        break;
    }
}

void StateBigClock::prepareFonts(int variant)
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

void StateBigClock::prepareNumbers()
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
