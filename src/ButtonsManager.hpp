#pragma once

#include <Arduino.h>
#include <ezButton.h>

#include <array>
#include <functional>

#include "Event.hpp"

class ButtonsManager
{
public:
    ButtonsManager()
    {
        for (auto &evBtn : m_evBtns)
        {
            evBtn.btn.setDebounceTime(10);
        }
    }

    void onClick(std::function<void(Event)> clbk)
    {
        m_clbk = clbk;
    }

    void update()
    {
        for (auto &evBtn : m_evBtns)
        {
            evBtn.btn.loop();
            if (evBtn.btn.isPressed())
            {
                evBtn.pressTimestamp = millis();
            }

            if (evBtn.btn.isReleased())
            {
                evBtn.shortActionFired = false;
                evBtn.longActionFired = false;
                evBtn.veryLongActionFired = false;
            }
        }

        if (isPressed(m_evBtns[0]))
        {
            m_clbk(Event::ACCEPT);
        }
        if (isLongAction(m_evBtns[0]))
        {
            m_clbk(Event::TIME_ADJUST);
        }
        if (isVeryLongAction(m_evBtns[0]))
        {
            m_clbk(Event::RESET_WIFI);
        }

        if (isPressed(m_evBtns[1]))
        {
            m_clbk(Event::POMODORO_CLOCK_SWITCH);
        }
        if (isLongAction(m_evBtns[1]))
        {
            m_clbk(Event::POMODORO_EXIT);
        }

        if (isPressed(m_evBtns[2]))
        {
            m_clbk(Event::POMODORO_START);
        }
        if (isLongAction(m_evBtns[2]))
        {
            m_clbk(Event::POMODORO_RESET);
        }

        if (isPressed(m_evBtns[3]))
        {
            m_clbk(Event::POMODORO_PAUSE);
        }
        if (isLongAction(m_evBtns[3]))
        {
            m_clbk(Event::POMODORO_SKIP);
        }
    }

private:
    struct EvBtn
    {
        ezButton btn;
        decltype(millis()) pressTimestamp = 0;
        bool shortActionFired = false;
        bool longActionFired = false;
        bool veryLongActionFired = false;
    };

    std::array<EvBtn, 4> m_evBtns{{{13}, {12}, {14}, {27}}};
    std::function<void(Event)> m_clbk;

    bool isLongAction(EvBtn &evBtn)
    {
        constexpr auto longActionTime = 1500;
        if (evBtn.btn.getState() == 0 && evBtn.longActionFired == false
            && millis() - evBtn.pressTimestamp > longActionTime)
        {
            evBtn.longActionFired = true;
            return true;
        }
        return false;
    }

    bool isVeryLongAction(EvBtn &evBtn)
    {
        constexpr auto veryLongActionTime = 5000;
        if (evBtn.btn.getState() == 0 && evBtn.veryLongActionFired == false
            && millis() - evBtn.pressTimestamp > veryLongActionTime)
        {
            evBtn.veryLongActionFired = true;
            return true;
        }
        return false;
    }

    bool isPressed(EvBtn &evBtn)
    {
        if (evBtn.btn.getState() == 0 && evBtn.shortActionFired == false)
        {
            evBtn.shortActionFired = true;
            return true;
        }
        return false;
    }
};
