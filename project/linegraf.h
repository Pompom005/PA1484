#pragma once
#include<iostream>
#include<string>
#include <lvgl.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <vector>
#include<string>
#include<algorithm>
// I need to include the file of the graf
using namespace std;

class Linegraf{
    private:
        lv_obj_t * obj1; 
        lv_obj_t * serie1;
        vector<float> valuesforthegraf;
        string location;
        float mx =0;
        float mn=0;
        // i need a pointer to the graf too 
    public:
        Linegraf(lv_obj_t* parent, const vector<float>& chosen, string name);
        void replacedata(float max_value, float value_min, int numberofdots);
        void adddatatolist(float data);
        int getminmaxsize(const vector<float>& grafvalues);
        void changelocation(string changed);
        void thehelpfunction();
};