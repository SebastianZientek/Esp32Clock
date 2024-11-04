#include "App.hpp"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiManager.h>
#include <ezButton.h>

#include <memory>

#include "DisplayDriver.hpp"
#include "State.hpp"
#include "StateClock.hpp"
#include "StatePomodoro.hpp"
#include "TimeManager.hpp"

namespace
{
WiFiManager wifiManager;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 1, 60000);
// TimeManager timeManager;
}  // namespace

struct Time
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

Time getTime()
{
    uint8_t s, m, h;
    timeClient.update();
    unsigned long unix_epoch = timeClient.getEpochTime();

    h = hour(unix_epoch);
    m = minute(unix_epoch);
    s = second(unix_epoch);

    return {h, m, s};
}

void App::init()
{
    m_displayDriver->init(21, 22, 23);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    m_displayDriver->setBrightness(50);
    m_displayDriver->setPos(0, 0);
    m_displayDriver->clear();

    m_clockDisplay.init();

    // button.setDebounceTime(50);
    m_stateKeeper = std::make_shared<StateKeeper>();
    m_stateKeeper->state = std::make_shared<StateClock>(m_stateKeeper, m_displayDriver);

    // m_displayDriver->print("Connecting");

    wifiManager.setConfigPortalTimeout(30);
    wifiManager.setConnectTimeout(10);
    wifiManager.setSaveConnectTimeout(30);
    wifiManager.setConnectRetries(10);

    do
    {
        // m_displayDriver->print(".");
        wifiManager.autoConnect("Esp32Clock");
    } while (WiFi.status() != WL_CONNECTED);

    timeClient.begin();
    // timeManager.init();

    ArduinoOTA.begin();

    m_displayDriver->setPos(0,0);
    m_displayDriver->print(WiFi.localIP().toString().c_str());
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    m_displayDriver->clear();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    m_buttonsMgr.onClick([&](Event ev)
    {
        static int pos = 0;
        pos = (pos + 1) % 3;
        m_displayDriver->setPos(pos, 1);
        m_displayDriver->printf("Event: %u", static_cast<int>(ev));
    });
}

void App::update()
{
    ArduinoOTA.handle();
    m_buttonsMgr.update();
    // timeManager.update();

    Time time = getTime();

    static auto clickTime = 0;
    m_clockDisplay.printTime(time.hour, time.minute, time.second, false);
    m_displayDriver->setPos(0, 1);
    // m_displayDriver->print(timeManager.getDateString().c_str());
}
