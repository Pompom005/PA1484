#include <LilyGo_AMOLED.h>
#include "ScaleableLabel.h"

    LV_FONT_DECLARE(arial_12);
    LV_FONT_DECLARE(arial_16);
    LV_FONT_DECLARE(arial_20);
    LV_FONT_DECLARE(arial_24);
    LV_FONT_DECLARE(arial_28);
    LV_FONT_DECLARE(arial_32);
    LV_FONT_DECLARE(arial_36);
    LV_FONT_DECLARE(arial_40);
    LV_FONT_DECLARE(arial_44);
    LV_FONT_DECLARE(arial_48);

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
        font = &arial_48;
    }
    else if(final_size > 1.6f)
    {
        font = &arial_44;
    }
    else if(final_size > 1.4f)
    {
        font = &arial_40;
    }
    else if(final_size > 1.2f)
    {
        font = &arial_36;
    }
    else if(final_size > 1.0f)
    {
        font = &arial_32;
    }
    else if(final_size > 0.8f)
    {
        font = &arial_28;
    }
    else if(final_size > 0.6f)
    {
        font = &arial_24;
    }
    else if(final_size > 0.4f)
    {
        font = &arial_20;
    }
    else if(final_size > 0.2f)
    {
        font = &arial_16;
    }
    else
    {
        font = &arial_12;
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