#include "BootScreen.h"

BootScreen::BootScreen()
    : screen(nullptr), logoLabel(nullptr), text1(nullptr), text2(nullptr), flash(nullptr) {}

void BootScreen::anim_cb(void* var, int32_t v) {
    lv_obj_set_y((lv_obj_t*)var, v);
}

void BootScreen::flash_cb(void* var, int32_t v) {
    lv_obj_set_style_bg_opa((lv_obj_t*)var, v, LV_PART_MAIN);
}

void BootScreen::init() {
    screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);

    logoLabel = lv_label_create(screen);
    lv_label_set_text(logoLabel, "Weather app");
    lv_obj_set_style_text_color(logoLabel, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(logoLabel, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_align(logoLabel, LV_ALIGN_TOP_MID, 0, -60);

    text1 = lv_label_create(screen);
    lv_label_set_text(text1, "v1.0");
    lv_obj_set_style_text_color(text1, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(text1, LV_ALIGN_BOTTOM_LEFT, 10, -30);

    text2 = lv_label_create(screen);
    lv_label_set_text(text2, "Grupp 9");
    lv_obj_set_style_text_color(text2, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(text2, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // white overlay used for flash
    flash = lv_obj_create(screen);
    lv_obj_set_size(flash, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(flash, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(flash, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(flash, 0, LV_PART_MAIN);
}

void BootScreen::show() {
    lv_scr_load(screen);

    // Drop animation
    lv_anim_t drop;
    lv_anim_init(&drop);
    lv_anim_set_var(&drop, logoLabel);
    lv_anim_set_exec_cb(&drop, anim_cb);
    lv_anim_set_values(&drop, -60, 80);
    lv_anim_set_time(&drop, 1000);
    lv_anim_set_path_cb(&drop, lv_anim_path_ease_out);
    lv_anim_start(&drop);

    // Flash animation (starts shortly after drop)
    lv_anim_t flashAnim;
    lv_anim_init(&flashAnim);
    lv_anim_set_var(&flashAnim, flash);
    lv_anim_set_exec_cb(&flashAnim, flash_cb);
    lv_anim_set_values(&flashAnim, 0, 255);
    lv_anim_set_time(&flashAnim, 100);
    lv_anim_set_delay(&flashAnim, 1000);
    lv_anim_set_playback_time(&flashAnim, 150);
    lv_anim_set_playback_delay(&flashAnim, 50);
    lv_anim_start(&flashAnim);
}

void BootScreen::setLogoText(const char* text) {
    lv_label_set_text(logoLabel, text);
}

void BootScreen::hide() {
    if (!screen) return;
    lv_obj_t* blank = lv_obj_create(nullptr);
    lv_scr_load(blank);
    lv_obj_del(screen);
    screen = nullptr;
}
