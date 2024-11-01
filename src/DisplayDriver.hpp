#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <inttypes.h>

class DisplayDriver
{
public:
    void init(uint8_t sio, uint8_t stb, uint8_t sck);
    void clear();
    void setBrightness(int brightness);
    void setPos(uint8_t x, uint8_t y);

    void print(char c);
    void print(const char *str);
    void printf(const char *fmt, ...);

    void createCustom(uint8_t location, uint8_t charmap[], int linesToPrint = -1);
    void printCustom(uint8_t id);

private:
    bool m_displayInitialized = false;

    uint8_t m_pinSio;
    uint8_t m_pinStb;
    uint8_t m_pinSck;

    void setup();
    void cmd(uint8_t data);
    uint8_t readCmd();
    uint8_t readAddress();
    void newLine();
    uint8_t read(uint8_t registerSelect);
    void write(uint8_t data, uint8_t registerSelect);
};

#endif  // _DISPLAY_H_
