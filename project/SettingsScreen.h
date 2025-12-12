#pragma once
#include "Dropdown.h"
#include <string>
#include <functional>
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"

class SettingsScreen
{
    private:
        lv_obj_t* button;
        lv_obj_t* close_image; //Image shown TO close, aka when screen is visible
        lv_obj_t* open_image; //Image shown TO open, aka when screen is not visible
        lv_obj_t* popup_screen; //Actual settings screen base

        lv_obj_t* save_button;
        lv_obj_t* load_button;

        //Labels to explain
        lv_obj_t* settings_label;
        lv_obj_t* locations_label;
        lv_obj_t* conditions_label;
        lv_obj_t* save_label;
        lv_obj_t* load_label;

        //Dropdowns
        Dropdown<DropdownStation>* locations_dropdown;
        Dropdown<DropdownParameter>* conditions_dropdown; 
        
        bool state = false; //True == open, false == closed

        static void OnButtonPressed(lv_event_t* event);
        void internal_button_pressed(); //Need access to the specific settings screen variables,  cant get from the static one.

        static void ButtonSave(lv_event_t* e);
        static void ButtonLoad(lv_event_t* e);

    public:
        SettingsScreen();

        void hide_on_tiles(); //Hides the settings screen along with its button
        void show_on_tiles(); //Shows the settings screen along with its button

        void add_listener(std::function<void(DropdownStation)> func);
        void add_listener(std::function<void(DropdownParameter)> func);

        void save_selection_as_default(); //Saves to preferences
        void load_default_values(); //Loads from preferences and overwrites the current dropdown values
};