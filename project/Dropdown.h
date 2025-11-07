#pragma once
#include <LilyGo_AMOLED.h>
#include<iostream>
#include <lvgl.h>
#include<vector>
#include<string>
#include<Arduino.h>
#include<LV_Helper.h>
#include <sstream>
using namespace std;

template <typename T>

class Dropdown{
  private:
    vector<T> choices;
    lv_obj_t * dropdownBox;
    string optionStr;
  public:
    Dropdown(const vector<T>& cities, lv_obj_t * parent);
    static void event_handler(lv_event_t * e);
    void screenpos(int x, int y);
    string makeittostring(const vector<T> & stad);

};