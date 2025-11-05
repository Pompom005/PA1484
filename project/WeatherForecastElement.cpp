#include <LilyGo_AMOLED.h> //MUST HAVE IN CPP???
#include "WeatherForecastElement.h"
#include <sstream>
#include <iomanip>

LV_IMG_DECLARE(Sunny);
LV_IMG_DECLARE(Cloudy);
LV_IMG_DECLARE(Rain);
LV_IMG_DECLARE(Thunder);
LV_IMG_DECLARE(Snow);

//Very good to set size and pos in constructor to avoid having to delete texts right now
WeatherForecastElement::WeatherForecastElement(lv_obj_t* parent_tile, float size)
{
    this->size = size;
    xPos = 0;
    yPos = 0;
    tile = parent_tile;
    CreateTextsAndSymbol();
    UpdateFontsOnTexts();
    SetDefaultValues(); //Just sets random 0 values.  
}

void WeatherForecastElement::CreateTextsAndSymbol()
{ 
    //Order matters for rendering order.
    //Symbol
    symbol = lv_img_create(tile);
    lv_obj_align(symbol, LV_ALIGN_CENTER, 0, 0);

    //Temperature
    temperature_text = lv_label_create(tile);
    lv_obj_align(temperature_text, LV_ALIGN_CENTER, 0, 0);

    //Location
    location_text = lv_label_create(tile);
    lv_obj_align(location_text, LV_ALIGN_CENTER, 0, 0);

    //Time
    time_text = lv_label_create(tile);
    lv_obj_align(time_text, LV_ALIGN_CENTER, 0, 0);

    //Symbol text
    symbol_text = lv_label_create(tile);
    lv_obj_align(symbol_text, LV_ALIGN_CENTER, 0, 0);
}

void WeatherForecastElement::UpdateFontsOnTexts()
{
    //Smaller font for smaller sizes
    const lv_font_t* font = nullptr;
    if(size > 0.8f)
    {
        font = &lv_font_montserrat_28;
    }
    else if(size > 0.6f)
    {
        font = &lv_font_montserrat_24;
    }
    else if(size > 0.4f)
    {
        font = &lv_font_montserrat_20;
    }
    else if(size > 0.2f)
    {
        font = &lv_font_montserrat_16;
    }
    else
    {
        font = &lv_font_montserrat_12;
    }

    //Temperature
    lv_obj_set_style_text_font(temperature_text, font, 0);

    //Location
    lv_obj_set_style_text_font(location_text, font, 0);

    //Time
    lv_obj_set_style_text_font(time_text, font, 0);

    //Symbol text
    lv_obj_set_style_text_font(symbol_text, font, 0);
}

void WeatherForecastElement::SetDefaultValues()
{
    SetValues(0, "0", "0", static_cast<WeatherType>(0));
    SetSize(size);
    SetPosition(xPos, yPos);
}

void WeatherForecastElement::SetValues(float temperature, const std::string& location, const std::string& time, WeatherType type)
{
    SetTemp(temperature);
    SetLocation(location);
    SetTime(time);
    SetWeatherType(type);
}

void WeatherForecastElement::SetTemp(float temperature)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << temperature;
    lv_label_set_text(temperature_text, (stream.str() + "°").c_str());
}

void WeatherForecastElement::SetLocation(const std::string &location)
{
    lv_label_set_text(location_text, location.c_str());
}

void WeatherForecastElement::SetTime(const std::string &time)
{
    lv_label_set_text(time_text, time.c_str());
}

void WeatherForecastElement::SetWeatherType(WeatherType type)
{
    lv_label_set_text(symbol_text, GetTypeName(type).c_str());
    switch (type)
    {
    case WeatherType::Sunny:
        lv_img_set_src(symbol, &Sunny);
        break;
    
    case WeatherType::Cloudy:
        lv_img_set_src(symbol, &Cloudy);
        break;

    case WeatherType::Rain:
        lv_img_set_src(symbol, &Rain);
        break;

    case WeatherType::Snow:
        lv_img_set_src(symbol, &Snow);
        break;

    case WeatherType::Thunder:
        lv_img_set_src(symbol, &Thunder);
        break;

    default:
        break;
    }
}

void WeatherForecastElement::SetPosition(float x, float y)
{
    float xSize = lv_obj_get_content_width(tile); //Size of x in pixels 
    float ySize = lv_obj_get_content_height(tile); //Size of y in pixels

    //Symbol
    lv_obj_set_pos(symbol, x*xSize, y*ySize); //Centered

    //Temp
    lv_obj_set_pos(temperature_text, x*xSize, y*ySize); //Centered

    //Location
    lv_obj_set_pos(location_text, x*xSize, (y - 0.40f * size)*ySize); //Above

    //Time
    lv_obj_set_pos(time_text, x*xSize, (y - 0.30f * size)*ySize); //Above

    //Symbol text
    lv_obj_set_pos(symbol_text, x*xSize, (y + 0.30f * size)*ySize); //Above

    xPos = x;
    yPos = y;
}

void WeatherForecastElement::SetSize(float size)
{
    float finalSize = lv_obj_get_content_width(tile) * size;

    //Symbol
    float zoomPercentage = (finalSize / lv_obj_get_content_width(tile)) * 0.9f;
    lv_img_set_zoom(symbol, 256 * zoomPercentage);

    //Size intervals for the text size:
    if(this->size != size)
    {
        UpdateFontsOnTexts();
            
        //Do this to update position relative to screen
        this->size = size;
        SetPosition(xPos, yPos);
    }
}
