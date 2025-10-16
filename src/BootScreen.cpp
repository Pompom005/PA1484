#include "BootScreen.h"
#include "Draw.h"

void showBootScreen(LilyGo_AMOLED &screen) {
    Draw draw(screen);
    draw.fillScreen(Draw::BLACK);
    draw.drawText(20, 60, "Lorem ipsum dolor sit amet consectetur adipiscing elit.", Draw::CYAN, 2);
    draw.drawText(20, 100, "Version 6.9", Draw::WHITE, 1);
}
