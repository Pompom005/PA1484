#pragma once
#include<iostream>
#include<string>
#include <lvgl.h>
// I need to include the file of the graf
using namespace std;

class Linegraf{
    private:
        int percentage;
        lv_obj_t * obj1;
        // i need a pointer to the graf too 
    public:
        Linegraf(lv_obj_t* parent, int num);
        void percentfunction();
};