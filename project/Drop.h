#pragma once
#include<iostream>
#include <lvgl.h>
#include<vector>
#include<string>
#include<Arduino.h>
#include <sstream>
using namespace std;

template <typename T>
class Dropdown
{
private:
    vector<T> choices;
    lv_obj_t * dropdownBox;
public:
    Dropdown(const vector<T>& cities): choices(cities){
        if(cities.size()==0){
            Serial.println("Empty list");
            return;
        }
        //dropdownBox = lv_dropdown_create(lv_scr_act());
        string optionStr;
        for(auto & city: cities){
            stringstream ss;
            ss << city;
            optionStr += ss.str() + "\n";
        }
        lv_dropdown_set_options(dropdownBox,optionStr.c_str());
        lv_obj_align(dropdownBox, LV_ALIGN_TOP_MID, 0, 20);
        lv_dropdown_set_selected(dropdownBox, 0);
        lv_obj_add_event_cb(dropdownBox, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    }

    
    static void event_handler(lv_event_t * e){
        lv_event_code_t code = lv_event_get_code(e);
        //::lv_obj_t * obj = ::lv_event_get_target_obj(e);
        if(code == LV_EVENT_VALUE_CHANGED) {
            char buf[32];
            lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
            LV_LOG_USER("Option: %s", buf);
        }


    }

};