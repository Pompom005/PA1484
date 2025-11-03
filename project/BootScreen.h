#pragma once
#include <lvgl.h>

class BootScreen {
private:
    lv_obj_t* screen;
    lv_obj_t* label;
    lv_obj_t* bar;

public:
    BootScreen();
    void init();
    void show();
    void updateProgress(int percent);
    void setText(const char* text);
    void hide();
};
