#pragma once
#include <lvgl.h>

class Throbber {
private:
    lv_obj_t* screen;
    lv_obj_t* spinner;
    lv_obj_t* label;

public:
    Throbber();
    void init(const char* text);
    void show();
    void hide();
};
