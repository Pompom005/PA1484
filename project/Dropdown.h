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
    lv_obj_t * dropdownBox = nullptr;
    string optionStr;

    std::vector<std::function<void(T)>> listeners;
  public:
    Dropdown(const vector<T>& cities, lv_obj_t * parent, int x, int y);
    static void event_handler(lv_event_t * e);
    void screenpos(int x, int y);
    string makeittostring(const vector<T> & stad);
    void addlistener(std::function<void(T)> func);
    string Dropdown<T>:: getValue();
};