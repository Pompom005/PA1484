#include "SettingsScreen.h"
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"

void SettingsScreen::OnButtonPressed(lv_event_t *event)
{
    SettingsScreen* settings = reinterpret_cast<SettingsScreen*>(event->user_data);
    settings->InternalButtonPressed();
}

void SettingsScreen::InternalButtonPressed()
{
    state = !state;
    if(state)
    {
        //Opened screen
        lv_obj_add_flag(open_image, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(close_image, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        //Closed screen
        lv_obj_add_flag(close_image, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(open_image, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);
    }
}

SettingsScreen::SettingsScreen()
{   
    //Popup, has to be before button and images
    popup_screen = lv_obj_create(lv_scr_act());
    lv_obj_align(popup_screen, LV_ALIGN_CENTER, 0, 0);
    float popup_height = lv_obj_get_content_height(lv_scr_act()) * 0.75f;
    float popup_width = lv_obj_get_content_width(lv_scr_act()) * 0.75f;
    lv_obj_set_content_height(popup_screen, popup_height);
    lv_obj_set_content_width(popup_screen, popup_width);

    //Button
    button = lv_btn_create(lv_scr_act());
    lv_obj_set_content_height(button, 40);
    lv_obj_set_content_width(button, 40);
    lv_obj_align(button, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(button, OnButtonPressed, LV_EVENT_CLICKED, this);
    //lv_obj_set_style_bg_color(button, lv_color_make(255, 19, 240), LV_STATE_DEFAULT); //Neon pink, superior color
    lv_obj_set_style_bg_color(button, lv_color_make(128, 128, 128), LV_STATE_DEFAULT); //Grey

    lv_obj_set_pos(button, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    //Images
    close_image = lv_label_create(lv_scr_act());
    lv_label_set_text(close_image, LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_font(close_image, &lv_font_montserrat_30, 0);
    lv_obj_align(close_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(close_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(close_image, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    open_image = lv_label_create(lv_scr_act());
    lv_label_set_text(open_image, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_font(open_image, &lv_font_montserrat_30, 0);
    lv_obj_align(open_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(open_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(open_image, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    //Dropdowns
    locations_dropdown = new Dropdown<DropdownStation>(SMHIStationsAndParameters::GetInstance().GetEligibleStations(SupportedParameter::AirTemperatureAverageDaily), popup_screen);
    conditions_dropdown = new Dropdown<DropdownParameter>(SMHIStationsAndParameters::GetInstance().GetParameters(), popup_screen);

    locations_dropdown->screenpos(popup_width * -0.25f, popup_height * -0.5f);
    conditions_dropdown->screenpos(popup_width * 0.25, popup_height * -0.5f);

    //Labels
    settings_label = lv_label_create(popup_screen);
    lv_obj_align(settings_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_font(settings_label, &lv_font_montserrat_30, 0);
    lv_label_set_text(settings_label, "Settings");

    locations_label = lv_label_create(popup_screen);
    lv_obj_align(locations_label, LV_ALIGN_BOTTOM_MID, popup_width * -0.25f, popup_height * -.6f);
    lv_obj_set_style_text_font(locations_label, &lv_font_montserrat_18, 0);
    lv_label_set_text(locations_label, "Location");

    conditions_label = lv_label_create(popup_screen);
    lv_obj_align(conditions_label, LV_ALIGN_BOTTOM_MID, popup_width * 0.25f, popup_height * -0.6f);
    lv_obj_set_style_text_font(conditions_label, &lv_font_montserrat_18, 0);
    lv_label_set_text(conditions_label, "Condition");

    //Set default values. Close popup and close image by default
    lv_obj_add_flag(close_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);

    //Important listener to change eligiblestations when we change parameter
    AddListenerToCondition([&](DropdownParameter newParameter)
    {
        locations_dropdown->UpdateList(SMHIStationsAndParameters::GetInstance().GetEligibleStations(newParameter.realParameter->enumParameterkey));
    });
}

void SettingsScreen::HideOnTiles()
{
    lv_obj_add_flag(button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(close_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(open_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);
}

void SettingsScreen::ShowOnTiles()
{
    lv_obj_clear_flag(button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(open_image, LV_OBJ_FLAG_HIDDEN);
}

void SettingsScreen::AddListenerToLocation(std::function<void(DropdownStation)> func)
{
    locations_dropdown->addlistener(func);
}

void SettingsScreen::AddListenerToCondition(std::function<void(DropdownParameter)> func)
{
    conditions_dropdown->addlistener(func);
}
