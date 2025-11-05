#include <LilyGo_AMOLED.h>
#include "ScaleableLabel.h"

ScaleableLabel::ScaleableLabel(lv_obj_t *parent, float size, float size_offset)
{
    this->size = size;

    label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    SetSizeOffset(size_offset); 
}
void ScaleableLabel::SetSize(float size)
{
    this->size = size;
    float final_size = size + size_offset;

    //Smaller font for smaller sizes
    const lv_font_t* font = nullptr;
    if(final_size > 1.8f)
    {
        font = &lv_font_montserrat_48;
    }
    else if(final_size > 1.6f)
    {
        font = &lv_font_montserrat_44;
    }
    else if(final_size > 1.4f)
    {
        font = &lv_font_montserrat_40;
    }
    else if(final_size > 1.2f)
    {
        font = &lv_font_montserrat_36;
    }
    else if(final_size > 1.0f)
    {
        font = &lv_font_montserrat_32;
    }
    else if(final_size > 0.8f)
    {
        font = &lv_font_montserrat_28;
    }
    else if(final_size > 0.6f)
    {
        font = &lv_font_montserrat_24;
    }
    else if(final_size > 0.4f)
    {
        font = &lv_font_montserrat_20;
    }
    else if(final_size > 0.2f)
    {
        font = &lv_font_montserrat_16;
    }
    else
    {
        font = &lv_font_montserrat_12;
    }

    lv_obj_set_style_text_font(label, font, 0);
}
void ScaleableLabel::SetSizeOffset(float offset)
{
    size_offset = offset;
    SetSize(size); //Update
}
void ScaleableLabel::SetText(const char *text)
{
    lv_label_set_text(label, text);
}
void ScaleableLabel::SetPosition(float x, float y)
{
    lv_obj_set_pos(label, x, y);
};