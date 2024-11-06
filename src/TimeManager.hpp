#pragma once

#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include <cstdint>
#include <string>
#include <Preferences.h>

class TimeManager
{
public:
    void init()
    {
        m_timeClient.begin();
        m_timeClient.update();

        m_preferences.begin("timer", true);
        m_hoursOffset = m_preferences.getInt("hoursOffset", 0);
        m_preferences.end();

        m_timeClient.setTimeOffset(secondsInHour * m_hoursOffset);
    }

    void update()
    {
        m_timeClient.update();
    }

    std::string getTimeString()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);

        char buffer[9];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", tm.Hour, tm.Minute, tm.Second);
        return std::string(buffer);
    }

    std::string getShortTimeString()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);

        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", tm.Hour, tm.Minute);
        return std::string(buffer);
    }

    std::string getDateString()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);

        char buffer[11];
        snprintf(buffer, sizeof(buffer), "%02d-%02d-%04d", tm.Day, tm.Month, tmYearToCalendar(tm.Year));
        return std::string(buffer);
    }

    int getHour()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);
        return tm.Hour;
    }

    int getMinute()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);
        return tm.Minute;
    }

    int getSecond()
    {
        time_t epochTime = m_timeClient.getEpochTime();
        tmElements_t tm;
        breakTime(epochTime, tm);
        return tm.Second;
    }

    time_t getEpochTime()
    {
        return m_timeClient.getEpochTime();
    }

    long getEpochMinutes()
    {
        return m_timeClient.getEpochTime() / 60;
    }

    void increaseHoursOffset()
    {
        m_hoursOffset = (m_hoursOffset + 1) % 24;
        m_timeClient.setTimeOffset(secondsInHour * m_hoursOffset);

        m_preferences.begin("timer", false);
        m_preferences.putInt("hoursOffset", m_hoursOffset);
        m_preferences.end();
    }

    int getHoursOffset()
    {
        return m_hoursOffset;
    }

private:
    WiFiUDP m_ntpUDP;
    NTPClient m_timeClient{m_ntpUDP, "europe.pool.ntp.org", secondsInHour * 1, 60000};
    Preferences m_preferences;

    static const auto secondsInHour = 3600;
    int m_hoursOffset = 0;
};
