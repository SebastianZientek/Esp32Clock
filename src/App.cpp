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

namespace
{
WiFiManager wifiManager;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 1, 60000);
ezButton button(13);
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
    m_displayDriver->setBrightness(50);
    m_displayDriver->setPos(0, 0);

    m_clockDisplay.init();

    button.setDebounceTime(50);
    m_state = std::make_shared<StateClock>(nullptr);

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

    ArduinoOTA.begin();

    m_displayDriver->setPos(0,0);
    m_displayDriver->print(WiFi.localIP().toString().c_str());
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    m_displayDriver->clear();
}

void App::update()
{
    ArduinoOTA.handle();

    button.loop();
    Time time = getTime();
    // m_clockDisplay.printTime(time.hour, time.minute, time.second);

    static auto clickTime = 0;

    if (button.isPressed())
    {
        clickTime = millis();
    }

    if (button.getState() == 0 && millis() - clickTime > 1000)
    {
        // m_clockDisplay.printTime(55, 55, 55);
    }
    else
    {
        m_clockDisplay.printTime(time.hour, time.minute, time.second, false);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
