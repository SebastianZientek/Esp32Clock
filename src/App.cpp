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
#include "StateDateClock.hpp"
#include "StateBigClock.hpp"
#include "StatePomodoro.hpp"
#include "TimeManager.hpp"

namespace
{
WiFiManager wifiManager;
}  // namespace

void App::init()
{
    m_displayDriver->init(21, 22, 23);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    m_displayDriver->setBrightness(50);
    m_displayDriver->setPos(0, 0);
    m_displayDriver->clear();

    m_timeManager = std::make_shared<TimeManager>();
    m_stateKeeper = std::make_shared<StateKeeper>();
    m_stateKeeper->state
        = std::make_shared<StateBigClock>(m_stateKeeper, m_displayDriver, m_timeManager);

    m_displayDriver->print("Connecting");
    wifiManager.setConfigPortalTimeout(30);
    wifiManager.setConnectTimeout(10);
    wifiManager.setSaveConnectTimeout(30);
    wifiManager.setConnectRetries(10);
    
    if(!wifiManager.getWiFiIsSaved())
    {
        m_displayDriver->print("WIFI not configured");
    }

    do
    {
        m_displayDriver->print(".");
        wifiManager.autoConnect("Esp32Clock");
    } while (WiFi.status() != WL_CONNECTED);

    m_timeManager->init();
    ArduinoOTA.begin();

    m_displayDriver->setPos(0, 0);
    m_displayDriver->print(WiFi.localIP().toString().c_str());
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    m_displayDriver->clear();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    m_buttonsMgr.onClick(
        [&](Event ev)
        {
            if (ev == Event::RESET_WIFI)
            {
                wifiManager.resetSettings();
                ESP.restart();
            }
            m_stateKeeper->state->onEvent(ev);
        });
}

void App::update()
{
    ArduinoOTA.handle();
    m_buttonsMgr.update();
    m_timeManager->update();

    m_stateKeeper->state->process();
}
