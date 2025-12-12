#include "SettingsScreen.h"
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"
#include <Preferences.h>

void SettingsScreen::OnButtonPressed(lv_event_t *event)
{
    SettingsScreen* settings = reinterpret_cast<SettingsScreen*>(event->user_data);
    settings->internal_button_pressed();
}

void SettingsScreen::internal_button_pressed()
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

void SettingsScreen::ButtonSave(lv_event_t *e)
{
    reinterpret_cast<SettingsScreen*>(e->user_data)->save_selection_as_default();
}

void SettingsScreen::ButtonLoad(lv_event_t *e)
{
    reinterpret_cast<SettingsScreen*>(e->user_data)->load_default_values();
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
    lv_obj_set_style_bg_color(button, lv_color_make(255, 19, 240), LV_STATE_DEFAULT); //Neon pink, superior color
    //lv_obj_set_style_bg_color(button, lv_color_make(128, 128, 128), LV_STATE_DEFAULT); //Grey

    lv_obj_set_pos(button, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    //Images
    close_image = lv_label_create(lv_scr_act());
    lv_label_set_text(close_image, LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_font(close_image, &arial_32, 0);
    lv_obj_align(close_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(close_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(close_image, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    open_image = lv_label_create(lv_scr_act());
    lv_label_set_text(open_image, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_font(open_image, &arial_32, 0);
    lv_obj_align(open_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(open_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(open_image, lv_obj_get_content_width(lv_scr_act()) * -0.4f, lv_obj_get_content_height(lv_scr_act()) * -0.4f);

    //Dropdowns
    locations_dropdown = new Dropdown<DropdownStation>(SMHIStationsAndParameters::instance().get_eligible_stations(SupportedParameter::AirTemperatureAverageDaily), popup_screen);
    conditions_dropdown = new Dropdown<DropdownParameter>(SMHIStationsAndParameters::instance().get_parameters(), popup_screen);

    locations_dropdown->screenpos(popup_width * -0.25f, popup_height * -0.5f);
    conditions_dropdown->screenpos(popup_width * 0.25, popup_height * -0.5f);

    //Save and Load defaults
    save_button = lv_btn_create(popup_screen);
    lv_obj_set_content_height(save_button, 40);
    lv_obj_set_content_width(save_button, 80);
    lv_obj_align(save_button, LV_ALIGN_BOTTOM_MID, popup_width * -0.25f, popup_height * -0.25f);
    lv_obj_add_event_cb(save_button, ButtonSave, LV_EVENT_CLICKED, this);
    lv_obj_set_style_bg_color(button, lv_color_make(255, 19, 240), LV_STATE_DEFAULT); //Neon pink, superior color
    //lv_obj_set_style_bg_color(save_button, lv_color_make(128, 128, 128), LV_STATE_DEFAULT); //Grey

    load_button = lv_btn_create(popup_screen);
    lv_obj_set_content_height(load_button, 40);
    lv_obj_set_content_width(load_button, 80);
    lv_obj_align(load_button, LV_ALIGN_BOTTOM_MID, popup_width * +0.25f, popup_height * -0.25f);
    lv_obj_add_event_cb(load_button, ButtonLoad, LV_EVENT_CLICKED, this);
    lv_obj_set_style_bg_color(button, lv_color_make(255, 19, 240), LV_STATE_DEFAULT); //Neon pink, superior color
    //lv_obj_set_style_bg_color(load_button, lv_color_make(128, 128, 128), LV_STATE_DEFAULT); //Grey

    //Labels
    settings_label = lv_label_create(popup_screen);
    lv_obj_align(settings_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_font(settings_label, &arial_32, 0);
    lv_label_set_text(settings_label, "Settings");

    locations_label = lv_label_create(popup_screen);
    lv_obj_align(locations_label, LV_ALIGN_BOTTOM_MID, popup_width * -0.25f, popup_height * -.6f);
    lv_obj_set_style_text_font(locations_label, &arial_20, 0);
    lv_label_set_text(locations_label, "Location");

    conditions_label = lv_label_create(popup_screen);
    lv_obj_align(conditions_label, LV_ALIGN_BOTTOM_MID, popup_width * 0.25f, popup_height * -0.6f);
    lv_obj_set_style_text_font(conditions_label, &arial_20, 0);
    lv_label_set_text(conditions_label, "Condition");

    save_label = lv_label_create(save_button);
    lv_obj_align(save_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(save_label, &arial_20, 0);
    lv_label_set_text(save_label, "Save Default");

    load_label = lv_label_create(load_button);
    lv_obj_align(load_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(load_label, &arial_20, 0);
    lv_label_set_text(load_label, "Load Default");

    //Set default values. Close popup and close image by default
    lv_obj_add_flag(close_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);

    //Important listener to change eligiblestations when we change parameter
    add_listener([&](DropdownParameter newParameter)
    {
        locations_dropdown->update_list(SMHIStationsAndParameters::instance().get_eligible_stations(newParameter.real_parameter->enum_parameter_key));
    });
}

void SettingsScreen::hide_on_tiles()
{
    lv_obj_add_flag(button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(close_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(open_image, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(popup_screen, LV_OBJ_FLAG_HIDDEN);
}

void SettingsScreen::show_on_tiles()
{
    lv_obj_clear_flag(button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(open_image, LV_OBJ_FLAG_HIDDEN);
}

void SettingsScreen::add_listener(std::function<void(DropdownStation)> func)
{
    locations_dropdown->add_listener(func);
}

void SettingsScreen::add_listener(std::function<void(DropdownParameter)> func)
{
    conditions_dropdown->add_listener(func);
}

void SettingsScreen::save_selection_as_default()
{
    Preferences pref;
    pref.begin("defaults", false);

    pref.putInt("defaultParam", conditions_dropdown->get_selected().real_parameter->smhi_parameter_key);
    pref.putInt("defaultStation", locations_dropdown->get_selected().real_station->key);
    pref.end();
}

void SettingsScreen::load_default_values()
{  
    Preferences pref;

    pref.begin("defaults", false);

    if(!pref.isKey("defaultParam")) //Doesnt exist
    {
        const SMHIParameter& param = SMHIStationsAndParameters::instance().get_parameter(SupportedParameter::AirTemperatureAverageDaily);
        pref.putInt("defaultParam", param.smhi_parameter_key);

        const std::vector<DropdownStation>& stations = SMHIStationsAndParameters::instance().get_eligible_stations(SupportedParameter::AirTemperatureAverageDaily);

        if(stations.size() > 0)
        {
            pref.putInt("defaultStation", stations[0].real_station->key);
        }
        else
        {
            pref.putInt("defaultStation", 0);
        }
        conditions_dropdown->set_selected(log2(static_cast<int>(SupportedParameter::AirTemperatureAverageDaily)));
        locations_dropdown->set_selected(0);
    }
    else
    {
        int paramKey = pref.getInt("defaultParam");
        int stationKey = pref.getInt("defaultStation");

        const std::vector<DropdownParameter> allParams = SMHIStationsAndParameters::instance().get_parameters();
        for(int i = 0; i < allParams.size(); i++)
        {
            //Using smhi key so its more accurate
            if(paramKey == allParams[i].real_parameter->smhi_parameter_key)
            {
                //This works because the dropdown will use the same list to initialize the values, so will be same index.
                conditions_dropdown->set_selected(i);

                const std::vector<DropdownStation>& stations = SMHIStationsAndParameters::instance().get_eligible_stations(allParams[i].real_parameter->enum_parameter_key);
                for(int j = 0; j < stations.size(); j++)
                {
                    if(stations[j].real_station->key == stationKey)
                    {
                        locations_dropdown->set_selected(j);
                        break;
                    }
                }
                //Default not found but should be okay.
                break;
            }
        }
    }
    pref.end();
}
