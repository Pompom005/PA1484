#pragma once
#include<iostream>
#include<string>
#include <lvgl.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <vector>
#include<string>
#include<algorithm>
#include "ParserStuff/SMHIHistoricalParser.h"
using namespace std;

class Linegraph{
    private:
        lv_obj_t * obj1; 
        lv_chart_series_t * serie1 = nullptr;
        std::vector<lv_coord_t> values;
        lv_obj_t* parent;
        float maxvalue;
        float minvalue;

        void get_the_max_min_value(const vector<lv_coord_t>& chosen);
    public:
        static void DrawCallbackEvent(lv_event_t* e);
        Linegraph(lv_obj_t* parent_tile, const vector<lv_coord_t>& chosen);
        void replace_data(const vector<lv_coord_t>& chosen, bool clearolddata);
};