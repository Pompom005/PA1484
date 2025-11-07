#pragma once
#include<iostream>
#include<string>
#include <lvgl.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
// I need to include the file of the graf
using namespace std;

class Linegraf{
    private:
        int percentage;
        lv_obj_t * obj1; 
        lv_obj_t * chart;
        // i need a pointer to the graf too 
    public:
        Linegraf(lv_obj_t* parent, int num);
        void percentfunction();
};