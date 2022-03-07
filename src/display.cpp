#include "display.hpp"

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#define NORITAKE_SIO 21
#define NORITAKE_STB 22
#define NORITAKE_SCK 23

#define PIN_LOW LOW
#define PIN_HIGH HIGH
#define PIN_OUTPUT OUTPUT
#define PIN_INPUT INPUT

static uint8_t displayInitialized = 0;

static void write(uint8_t data, uint8_t registerSelect)
{
    uint8_t value = 0xf8 + 2 * registerSelect;

    digitalWrite(NORITAKE_STB, PIN_LOW);
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(NORITAKE_SCK, PIN_LOW);
        digitalWrite(NORITAKE_SIO, value & i);
        digitalWrite(NORITAKE_SCK, PIN_HIGH);
    }

    value = data;
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(NORITAKE_SCK, PIN_LOW);
        digitalWrite(NORITAKE_SIO, value & i);
        digitalWrite(NORITAKE_SCK, PIN_HIGH);
    }
    digitalWrite(NORITAKE_STB, PIN_HIGH);
}

static uint8_t read(uint8_t registerSelect)
{
    uint8_t data = 0xfc + 2 * registerSelect;

    digitalWrite(NORITAKE_STB, PIN_LOW);
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(NORITAKE_SCK, PIN_LOW);
        digitalWrite(NORITAKE_SIO, data & i);
        digitalWrite(NORITAKE_SCK, PIN_HIGH);
    }

    pinMode(NORITAKE_SIO, PIN_INPUT);
    delayMicroseconds(1);

    for (uint8_t i = 0; i < 8; ++i)
    {
        digitalWrite(NORITAKE_SCK, PIN_LOW);
        delayMicroseconds(1);
        digitalWrite(NORITAKE_SCK, PIN_HIGH);

        data <<= 1;
        if (digitalRead(NORITAKE_SIO)) data |= 1;
    }

    digitalWrite(NORITAKE_STB, PIN_HIGH);
    pinMode(NORITAKE_SIO, PIN_OUTPUT);
    return data;
}

static void setupDisplay()
{
    pinMode(NORITAKE_SIO, PIN_OUTPUT);
    pinMode(NORITAKE_STB, PIN_OUTPUT);
    pinMode(NORITAKE_SCK, PIN_OUTPUT);

    digitalWrite(NORITAKE_SIO, PIN_HIGH);
    digitalWrite(NORITAKE_STB, PIN_HIGH);
    digitalWrite(NORITAKE_SCK, PIN_HIGH);

    delayMicroseconds(11000);

    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set

    delayMicroseconds(110);
}

static void cmd(uint8_t data)
{
    delayMicroseconds(54);
    write(data, 0);
}

static uint8_t readCmd()
{
    uint8_t data = read(0);
    delayMicroseconds(5);
    return data;
}

static uint8_t readAddress()
{
    return readCmd() & ~0x80;
}

static void newLine()
{
    uint8_t addr = readAddress();

    if (addr < DISPLAY_LINE1_ADDR + 20) addr = DISPLAY_LINE2_ADDR;
    else if (addr <= DISPLAY_LINE3_ADDR + 20)
        addr = DISPLAY_LINE4_ADDR;
    else if (addr < DISPLAY_LINE2_ADDR + 20)
        addr = DISPLAY_LINE3_ADDR;
    else
        addr = DISPLAY_LINE1_ADDR;
    cmd(DISPLAY_DDRAM | addr);

    delayMicroseconds(5);
}

void displayInit()
{
    setupDisplay();

    cmd(DISPLAY_OFF);
    delayMicroseconds(60);

    cmd(DISPLAY_CLEAR);
    delayMicroseconds(4000);

    cmd(DISPLAY_MODE_SET);
    delayMicroseconds(60);

    cmd(DISPLAY_SETTINGS | DISPLAY_ON);
    delayMicroseconds(60);

    displayInitialized = 1;
}

void displaySetBrightness(int brightness)
{
    if (brightness <= 0 || brightness > 100) return;
    cmd(0x30);
    delayMicroseconds(5);
    write((100 - brightness) / 25, 1);
    delayMicroseconds(5);
}

void displaySetPos(uint8_t x, uint8_t y)
{
    while (!displayInitialized)
    {
        taskYIELD();
    }

    switch (y)
    {
    case 0:
        cmd((DISPLAY_DDRAM | DISPLAY_LINE1_ADDR) + x);
        break;
    case 1:
        cmd((DISPLAY_DDRAM | DISPLAY_LINE2_ADDR) + x);
        break;
    case 2:
        cmd((DISPLAY_DDRAM | DISPLAY_LINE3_ADDR) + x);
        break;
    case 3:
        cmd((DISPLAY_DDRAM | DISPLAY_LINE4_ADDR) + x);
        break;
    }
    delayMicroseconds(500);
}

void displayPrint(const char *str)
{
    while (!displayInitialized)
    {
        taskYIELD();
    }

    while (*str)
    {
        if (*str == '\n') newLine();
        else
            write(*str, 1);
        str++;
    }
}

void displayPrintf(const char *fmt, ...)
{
    while (!displayInitialized)
    {
        taskYIELD();
    }

    char buffer[DISPLAY_BUFFER_SIZE];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, DISPLAY_BUFFER_SIZE, fmt, args);
    va_end(args);

    displayPrint(buffer);
}

void displayClear()
{
    write(0x01, 0);
    delayMicroseconds(5);
}
