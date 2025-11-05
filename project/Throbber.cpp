#include "Throbber.h"

Throbber::Throbber() : screen(nullptr), spinner(nullptr), label(nullptr) {}

void Throbber::init(const char* text) {
    screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);

    spinner = lv_spinner_create(screen, 1000, 60);  // period, arc length
    lv_obj_set_size(spinner, 60, 60);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_arc_color(spinner, lv_color_white(), LV_PART_INDICATOR);

    label = lv_label_create(screen);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 40);
}

void Throbber::show() {
    lv_scr_load(screen);
}

void Throbber::hide() {
    if (!screen) return;
    lv_obj_t* blank = lv_obj_create(nullptr);
    lv_scr_load(blank);
    lv_obj_del(screen);
    screen = nullptr;
}
