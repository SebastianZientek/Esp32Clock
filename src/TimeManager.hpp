#pragma once

#include <NTPClient.h>

#include <cstdint>
#include <string>

class TimeManager
{
public:
    void init()
    {
        m_timeClient.begin();
        m_timeClient.update();
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

private:
    WiFiManager m_wifiManager;
    WiFiUDP m_ntpUDP;
    NTPClient m_timeClient{m_ntpUDP, "europe.pool.ntp.org", 3600 * 1, 60000};
};
