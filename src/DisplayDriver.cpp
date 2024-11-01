#include "DisplayDriver.hpp"

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#define PIN_LOW LOW
#define PIN_HIGH HIGH
#define PIN_OUTPUT OUTPUT
#define PIN_INPUT INPUT

#define DISPLAY_DDRAM 0x80
#define DISPLAY_LINE1_ADDR 0x00
#define DISPLAY_LINE2_ADDR 0x40
#define DISPLAY_LINE3_ADDR 0x14
#define DISPLAY_LINE4_ADDR 0x54

#define DISPLAY_SET_COMMAND_FOR_8_BIT_MODE 0x38
#define DISPLAY_SET_COMMAND_FOR_4_BIT_MODE 0x28
#define DISPLAY_FUNCTION_SET 0x30
#define DISPLAY_ENTER_4_BIT_MODE 0x2
#define DISPLAY_MODE_SET 0x06
#define DISPLAY_SETTINGS 0x08

#define DISPLAY_CLEAR 0x01
#define DISPLAY_OFF 0x08
#define DISPLAY_ON 0x04
#define DISPLAY_CURSOR_ON 0x02
#define DISPLAY_CURSOR_BLINK 0x01

#define DISPLAY_BUFFER_SIZE 40

void DisplayDriver::init(uint8_t sio, uint8_t stb, uint8_t sck)
{
    m_pinSio = sio;
    m_pinStb = stb;
    m_pinSck = sck;

    setup();

    cmd(DISPLAY_OFF);
    delayMicroseconds(60);

    cmd(DISPLAY_CLEAR);
    delayMicroseconds(4000);

    cmd(DISPLAY_MODE_SET);
    delayMicroseconds(60);

    cmd(DISPLAY_SETTINGS | DISPLAY_ON);
    delayMicroseconds(60);

    m_displayInitialized = 1;
}

void DisplayDriver::clear()
{
    write(0x01, 0);
    delayMicroseconds(5);
}

void DisplayDriver::setBrightness(int brightness)
{
    if (brightness <= 0 || brightness > 100) return;
    cmd(0x30);
    delayMicroseconds(5);
    write((100 - brightness) / 25, 1);
    delayMicroseconds(5);
}

void DisplayDriver::setPos(uint8_t x, uint8_t y)
{
    while (!m_displayInitialized)
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

void DisplayDriver::print(char c)
{
    while (!m_displayInitialized)
    {
        taskYIELD();
    }

    write(c, 1);
}

void DisplayDriver::print(const char *str)
{
    while (!m_displayInitialized)
    {
        taskYIELD();
    }

    while (*str)
    {
        if (*str == '\n')
            newLine();
        else
            write(*str, 1);
        str++;
    }
}

void DisplayDriver::printf(const char *fmt, ...)
{
    while (!m_displayInitialized)
    {
        taskYIELD();
    }

    char buffer[DISPLAY_BUFFER_SIZE];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, DISPLAY_BUFFER_SIZE, fmt, args);
    va_end(args);

    print(buffer);
}

void DisplayDriver::createCustom(uint8_t location, uint8_t charmap[], int linesToPrint)
{
    uint8_t LCD_SETCGRAMADDR = 0x40;
    location &= 0x7;
    cmd(LCD_SETCGRAMADDR | (location << 3));

    for (int i = 0; i < 8; ++i)
    {
        if (linesToPrint == -1)
        {
            write(charmap[i], 1);
        }
        else
        {
            if (i % 2 == linesToPrint)
            {
                write(charmap[i], 1);
            }
            else {
                write(0, 1);
            }
        }
    }
}

void DisplayDriver::printCustom(uint8_t id)
{
    write(id, 1);
}

// Private

void DisplayDriver::setup()
{
    pinMode(m_pinSio, PIN_OUTPUT);
    pinMode(m_pinStb, PIN_OUTPUT);
    pinMode(m_pinSck, PIN_OUTPUT);

    digitalWrite(m_pinSio, PIN_HIGH);
    digitalWrite(m_pinStb, PIN_HIGH);
    digitalWrite(m_pinSck, PIN_HIGH);

    delayMicroseconds(11000);

    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set
    digitalWrite(0x30, false);  // 8-bit function set

    delayMicroseconds(110);
}

void DisplayDriver::cmd(uint8_t data)
{
    delayMicroseconds(54);
    write(data, 0);
}

uint8_t DisplayDriver::readCmd()
{
    uint8_t data = read(0);
    delayMicroseconds(5);
    return data;
}

uint8_t DisplayDriver::readAddress()
{
    return readCmd() & ~0x80;
}

void DisplayDriver::newLine()
{
    uint8_t addr = readAddress();

    if (addr < DISPLAY_LINE1_ADDR + 20)
        addr = DISPLAY_LINE2_ADDR;
    else if (addr <= DISPLAY_LINE3_ADDR + 20)
        addr = DISPLAY_LINE4_ADDR;
    else if (addr < DISPLAY_LINE2_ADDR + 20)
        addr = DISPLAY_LINE3_ADDR;
    else
        addr = DISPLAY_LINE1_ADDR;
    cmd(DISPLAY_DDRAM | addr);

    delayMicroseconds(5);
}

uint8_t DisplayDriver::read(uint8_t registerSelect)
{
    uint8_t data = 0xfc + 2 * registerSelect;

    digitalWrite(m_pinStb, PIN_LOW);
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(m_pinSck, PIN_LOW);
        digitalWrite(m_pinSio, data & i);
        digitalWrite(m_pinSck, PIN_HIGH);
    }

    pinMode(m_pinSio, PIN_INPUT);
    delayMicroseconds(1);

    for (uint8_t i = 0; i < 8; ++i)
    {
        digitalWrite(m_pinSck, PIN_LOW);
        delayMicroseconds(1);
        digitalWrite(m_pinSck, PIN_HIGH);

        data <<= 1;
        if (digitalRead(m_pinSio)) data |= 1;
    }

    digitalWrite(m_pinStb, PIN_HIGH);
    pinMode(m_pinSio, PIN_OUTPUT);
    return data;
}

void DisplayDriver::write(uint8_t data, uint8_t registerSelect)
{
    uint8_t value = 0xf8 + 2 * registerSelect;

    digitalWrite(m_pinStb, PIN_LOW);
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(m_pinSck, PIN_LOW);
        digitalWrite(m_pinSio, value & i);
        digitalWrite(m_pinSck, PIN_HIGH);
    }

    value = data;
    for (uint8_t i = 0x80; i; i >>= 1)
    {
        digitalWrite(m_pinSck, PIN_LOW);
        digitalWrite(m_pinSio, value & i);
        digitalWrite(m_pinSck, PIN_HIGH);
    }
    digitalWrite(m_pinStb, PIN_HIGH);
}