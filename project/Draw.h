#pragma once
#include "LilyGo_AMOLED.h"

class Draw {
public:
    explicit Draw(LilyGo_AMOLED &displayRef);

    void fillScreen(uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void drawText(int x, int y, const char *text, uint16_t color, int scale = 1);
    void drawLine(int x, int y, uint16_t color);

    // Common colors

    static const uint16_t BLACK  = 0x0000;
    static const uint16_t WHITE  = 0xFFFF;
    static const uint16_t RED    = 0xF800;
    static const uint16_t GREEN  = 0x07E0;
    static const uint16_t BLUE   = 0x001F;
    static const uint16_t CYAN   = 0x07FF;
    static const uint16_t YELLOW = 0xFFE0;

private:
    LilyGo_AMOLED &scr;
    void drawChar(int x, int y, char c, uint16_t color, int scale);
};