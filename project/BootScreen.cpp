#include "BootScreen.h"

BootScreen::BootScreen() : screen(nullptr), label(nullptr), bar(nullptr) {}

void BootScreen::init() {
    screen = lv_obj_create(nullptr);

    label = lv_label_create(screen);
    lv_label_set_text(label, "Starting...");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);

    bar = lv_bar_create(screen);
    lv_obj_set_size(bar, 200, 20);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 20);
    lv_bar_set_range(bar, 0, 100);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);
}

void BootScreen::show() {
    lv_scr_load(screen);
}

void BootScreen::updateProgress(int percent) {
    lv_bar_set_value(bar, percent, LV_ANIM_ON);
}

void BootScreen::setText(const char* text) {
    lv_label_set_text(label, text);
}

void BootScreen::hide() {
    if (!screen) return;
    // Create a clean default screen to replace it
    lv_obj_t* blank = lv_obj_create(nullptr);
    lv_scr_load(blank);
    lv_obj_del(screen);
    screen = nullptr;
}
