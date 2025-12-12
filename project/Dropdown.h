#pragma once
#include <LilyGo_AMOLED.h>
#include<iostream>
#include <lvgl.h>
#include<vector>
#include<string>
#include<Arduino.h>
#include<LV_Helper.h>
#include <sstream>
#include <functional>
using namespace std;

template <typename T>

class Dropdown{
  private:
    vector<T> choices;
    lv_obj_t * dropdown_box = nullptr;
    string option_str;

    std::vector<std::function<void(T)>> listeners;
  public:
    Dropdown(const vector<T>& cities, lv_obj_t * parent, int x = 0, int y = 0);
    static void event_handler(lv_event_t * e);
    void screenpos(int x, int y);
    string make_it_to_string(const vector<T> & stad); //Converts all the vector elements to a string that will be used in the dropdown. (formatted: str\n str\nstr\n)
    void add_listener(std::function<void(T)> func); //Add a listener to the event callback of the dropdown
    void update_list(const vector<T>& cities);
    void set_selected(int index);
    T get_selected();
};