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
        void InternalButtonPressed();

        static void ButtonSave(lv_event_t* e);
        static void ButtonLoad(lv_event_t* e);

    public:
        SettingsScreen();

        void HideOnTiles();
        void ShowOnTiles();

        void AddListenerToLocation(std::function<void(DropdownStation)> func);
        void AddListenerToCondition(std::function<void(DropdownParameter)> func);

        void SaveSelectionAsDefault();
        void LoadDefaultValues();
};

//Create button on scr, not tile
//Create settings screen rect
//Create label + dropdowns on the settings screen rect
//Make button open/close screen
//Default state should be closed
//Allow us to "sign up" to setting changes, maybe observer pattern or something