#pragma once
#include "LV_Helper.h"

class ScaleableLabel
{
    private:
        lv_obj_t* label;
        float size_offset;
        float size;
    
    public:
        ScaleableLabel(lv_obj_t* parent, float size = 1.0f, float size_offset = 0.0f);

        void set_position(float x, float y); //Pixels on screen
        void set_size(float size); //Relative to the screen in percentage;
        void set_size_offset(float offset); //Shrinks the text additionally, basically lowering the "base" font; Can be bigger aswell, but I have a random limit
        void set_text(const char* text);
};