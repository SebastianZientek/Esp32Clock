#pragma once

#include "State.hpp"
#include "StateBigClock.hpp"

class StatePomodoro : public State
{
public:
    StatePomodoro(std::shared_ptr<StateKeeper> stateKeeper,
                  std::shared_ptr<DisplayDriver> displayDriver,
                  std::shared_ptr<TimeManager> timeManager)
        : State(stateKeeper)
        , m_displayDriver(displayDriver)
        , m_timeManager(timeManager)
    {
        m_displayDriver->clear();
    }

    void process() override
    {
        m_displayDriver->setPos(0, 0);
        if (m_mode == Mode::WORK_WAIT)
        {
            m_displayDriver->setPos(0, 0);
            m_displayDriver->print("START");
            m_displayDriver->setPos(0, 1);
            m_displayDriver->print("WORK?");
            m_secondsLeft = workTime;
        }
        if (m_mode == Mode::BREAK_WAIT)
        {
            m_displayDriver->setPos(0, 0);
            m_displayDriver->print("START");
            m_displayDriver->setPos(0, 1);
            m_displayDriver->print("BREAK?");
            m_secondsLeft = breakTime;
        }
        if (m_mode == Mode::WORK_PAUSE)
        {
            m_displayDriver->print("WORK PAUSED");
        }
        if (m_mode == Mode::BREAK_PAUSE)
        {
            m_displayDriver->print("BREAK PAUSED?");
        }
        else if (m_mode == Mode::WORK)
        {
            m_displayDriver->print("WORK");
            m_secondsLeft = m_endTimestamp - m_timeManager->getEpochTime();

            if (m_secondsLeft <= 0)
            {
                endSessionNotify();
                m_mode = Mode::BREAK_WAIT;
            }
        }
        else if (m_mode == Mode::BREAK)
        {
            m_displayDriver->print("BREAK");
            m_secondsLeft = m_endTimestamp - m_timeManager->getEpochTime();

            if (m_secondsLeft <= 0)
            {
                endSessionNotify();
                m_mode = Mode::WORK_WAIT;
            }
        }

        long minLeft = m_secondsLeft / 60;
        long secLeft = m_secondsLeft % 60;
        m_displayDriver->setPos(8, 1);
        m_displayDriver->printf("%02d:%02d", minLeft, secLeft);

        // Show current hour
        m_displayDriver->setPos(15, 0);
        m_displayDriver->print(m_timeManager->getShortTimeString().c_str());
    }

    void onEvent(Event event) override
    {
        m_displayDriver->clear();
        switch (event)
        {
        case Event::POMODORO_EXIT:
            changeState<StateBigClock>(m_displayDriver, m_timeManager);
            break;
        case Event::POMODORO_START:
            if (m_mode == Mode::WORK_WAIT)
            {
                startWork();
            }

            if (m_mode == Mode::BREAK_WAIT)
            {
                startBreak();
            }
            break;
        case Event::POMODORO_SKIP:
            m_secondsLeft = 0;
            switch (m_mode)
            {
            case Mode::WORK_WAIT:
                m_mode = Mode::BREAK_WAIT;
                break;
            case Mode::WORK_PAUSE:
                m_mode = Mode::BREAK_WAIT;
                m_secondsLeft = breakTime;
                break;
            case Mode::WORK:
                startBreak();
                break;
            case Mode::BREAK_WAIT:
                m_mode = Mode::WORK_WAIT;
                break;
            case Mode::BREAK_PAUSE:
                m_mode = Mode::WORK_WAIT;
                m_secondsLeft = workTime;
                break;
            case Mode::BREAK:
                startWork();
                break;
            }
            break;
        case Event::POMODORO_RESET:
            switch (m_mode)
            {
            case Mode::WORK:
                m_mode = Mode::WORK_WAIT;
                m_secondsLeft = 0;
                break;
            case Mode::BREAK:
                m_mode = Mode::BREAK_WAIT;
                m_secondsLeft = 0;
                break;
            }
            break;
        case Event::POMODORO_PAUSE:
            switch (m_mode)
            {
            case Mode::WORK:
                m_mode = Mode::WORK_PAUSE;
                break;
            case Mode::BREAK:
                m_mode = Mode::BREAK_PAUSE;
                break;
            case Mode::WORK_PAUSE:
                startWork(m_secondsLeft);
                break;
            case Mode::BREAK_PAUSE:
                startBreak(m_secondsLeft);
                break;
            }

            break;
        };
    }

private:
    std::shared_ptr<DisplayDriver> m_displayDriver;
    std::shared_ptr<TimeManager> m_timeManager;
    static const long workTime = 25 * 60;  // 25 minutes
    static const long breakTime = 5 * 60;  // 5 minutes
    long m_endTimestamp = 0;
    long m_secondsLeft = 0;

    enum class Mode
    {
        WORK_WAIT,
        WORK,
        BREAK_WAIT,
        BREAK,
        WORK_PAUSE,
        BREAK_PAUSE,
    } m_mode
        = Mode::WORK_WAIT;

    void startWork(long left = workTime)
    {
        m_mode = Mode::WORK;
        m_secondsLeft = left;
        m_endTimestamp = m_timeManager->getEpochTime() + m_secondsLeft;
    }

    void startBreak(long left = breakTime)
    {
        m_mode = Mode::BREAK;
        m_secondsLeft = left;
        m_endTimestamp = m_timeManager->getEpochTime() + m_secondsLeft;
    }

    void endSessionNotify()
    {
        ledcSetup(0, 800, 8);
        ledcAttachPin(4, 0);
    
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                ledcWrite(0, 128);
                delay(80);
                ledcWrite(0, 0);
                delay(80);
            }
            delay(100);
        }
    }
};
