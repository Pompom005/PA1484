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
// I need to include the file of the graf
using namespace std;

class Linegraf{
    private:
        lv_obj_t * obj1; 
        lv_chart_series_t * serie1 = nullptr;
        std::vector<lv_coord_t> values;
        // i need a pointer to the graf too 
    public:
        Linegraf(lv_obj_t* parent_tile, const vector<lv_coord_t>& chosen);
        void replacedata(const vector<lv_coord_t>& chosen, bool clearolddata);
        // void replacedata(int numberofdots, const vector<HistoricalDataPoint>& chosen);
        // void setDataFromSMHI(vector<HistoricalDataPoint>& history );

};