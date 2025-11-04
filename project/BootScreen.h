#pragma once
#include <lvgl.h>

class BootScreen {
private:
    lv_obj_t* screen;
    lv_obj_t* logoLabel;
    lv_obj_t* text1;
    lv_obj_t* text2;
    lv_obj_t* flash;

    static void anim_cb(void* var, int32_t v);
    static void flash_cb(void* var, int32_t v);

public:
    BootScreen();
    void init();
    void show();
    void setLogoText(const char* text);
    void hide();
};
