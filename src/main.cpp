#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiManager.h>

#include <string>

#include "App.hpp"

App app;

void setup()
{
    app.init();
}

void loop()
{
    app.update();
}
