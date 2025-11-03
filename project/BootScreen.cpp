#include "BootScreen.h"
#include "Draw.h"

void showBootScreen(LilyGo_AMOLED &screen) {
    Draw draw(screen);
    draw.fillScreen(Draw::BLACK);
    draw.drawText(20, 60, "Grupp 9", Draw::CYAN, 2);
    draw.drawText(20, 100, "Version 0.1", Draw::WHITE, 1);
}
