#include <DHT_U.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiManager.h>

#include "display.hpp"

WiFiManager wifiManager;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 3600, 60000);

#define DHTPIN 4
#define DHTTYPE DHT22
float temperature, humidity;

DHT_Unified dht(DHTPIN, DHTTYPE);

struct Time
{
    byte hour;
    byte minute;
    byte second;
};

Time getTime()
{
    byte s, m, h;
    timeClient.update();
    unsigned long unix_epoch = timeClient.getEpochTime();

    h = hour(unix_epoch);
    m = minute(unix_epoch);
    s = second(unix_epoch);

    return {h, m, s};
}

void printSensor()
{
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        displayPrint("--C");
    }
    else
    {
        temperature = event.temperature;
        displayPrintf("%02dC ", int(event.temperature));
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        displayPrintf("--%");
    }
    else
    {
        humidity = event.relative_humidity;
        displayPrintf("%02d%%", int(event.relative_humidity));
    }
}

void setup()
{
    displayInit();
    delayMicroseconds(5);
    displaySetBrightness(50);
    delayMicroseconds(5);

    displaySetPos(0, 0);
    displayPrint("Connecting");

    wifiManager.setConfigPortalTimeout(30);
    wifiManager.setConnectTimeout(10);
    wifiManager.setSaveConnectTimeout(30);
    wifiManager.setConnectRetries(10);

    do {
        displayPrint(".");
        wifiManager.autoConnect("Esp32Clock");
    } while(WiFi.status() != WL_CONNECTED);

    timeClient.begin();
    dht.begin();
}

byte last_minute = 255;
void loop()
{
    Time time = getTime();

    if (last_minute != time.minute) displayClear();
    last_minute = time.minute;

    displaySetPos(time.minute % 13, 0);
    displayPrintf("%02d:%02d:%02d", time.hour, time.minute, time.second);

    displaySetPos(time.minute % 14, 1);
    printSensor();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
