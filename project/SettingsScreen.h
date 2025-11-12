#pragma once
#include "Dropdown.h"
#include <string>
#include <functional>

class SettingsScreen
{
    private:
        lv_obj_t* button;
        lv_obj_t* close_image; //Image shown TO close, aka when screen is visible
        lv_obj_t* open_image; //Image shown TO open, aka when screen is not visible
        lv_obj_t* popup_screen; //Actual settings screen base


        //Labels to explain
        lv_obj_t* settings_label;
        lv_obj_t* locations_label;
        lv_obj_t* conditions_label;

        //Dropdowns
        Dropdown<std::string>* locations_dropdown;
        Dropdown<std::string>* conditions_dropdown; 
        
        bool state = false; //True == open, false == closed

        static void OnButtonPressed(lv_event_t* event);
        void InternalButtonPressed();

    public:
        SettingsScreen();

        void AddListenerToLocation(std::function<void(std::string)> func);
        void AddListenerToCondition(std::function<void(std::string)> func);
};

//Create button on scr, not tile
//Create settings screen rect
//Create label + dropdowns on the settings screen rect
//Make button open/close screen
//Default state should be closed
//Allow us to "sign up" to setting changes, maybe observer pattern or something