#include <LilyGo_AMOLED.h> //MUST HAVE IN CPP???
#include "WeatherForecastElement.h"
#include <sstream>
#include <iomanip>

//Very good to set size and pos in constructor to avoid having to delete texts right now
WeatherForecastElement::WeatherForecastElement(lv_obj_t* parent_tile, float size)
{
    this->size = size;
    xPos = 0;
    yPos = 0;
    tile = parent_tile;
    create_texts_and_symbol();
    set_default_values(); //Just sets random 0 values.  
}

void WeatherForecastElement::create_texts_and_symbol()
{ 
    //Order matters for rendering order.
    //Symbol
    symbol = lv_img_create(tile);
    lv_obj_align(symbol, LV_ALIGN_CENTER, 0, 0);

    //Temperature
    temperature_text = new ScaleableLabel(tile, 1.0f, 0.6f);
    temperature_text->set_size(size);

    //Location
    location_text = new ScaleableLabel(tile, 1.0f, -0.2f);
    location_text->set_size(size);

    //Time
    time_text = new ScaleableLabel(tile, 1.0f, -0.2f);
    time_text->set_size(size);

    //Symbol text
    symbol_text = new ScaleableLabel(tile);
    symbol_text->set_size(size);
}

void WeatherForecastElement::set_default_values()
{
    set_values(0, "0", "0", static_cast<WeatherType>(0));
    set_size(size);
    set_position(xPos, yPos);
}

void WeatherForecastElement::set_values(float temperature, const std::string& location, const std::string& time, WeatherType type)
{
    set_temp(temperature);
    set_location(location);
    set_time(time);
    set_weather_type(type);
}

void WeatherForecastElement::set_temp(float temperature)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << temperature;
    temperature_text->set_text((stream.str() + "°").c_str());
}

void WeatherForecastElement::set_location(const std::string &location)
{
    location_text->set_text(location.c_str());
    int referenceTextAmount = 12; //This amount of chars are a good fit with 0.0f offset
    int locationAmount = location.length();

    int diff = referenceTextAmount - locationAmount;
    location_text->set_size_offset((float)diff * 0.1f);
}

void WeatherForecastElement::set_time(const std::string &time)
{
    time_text->set_text(time.c_str());
}

void WeatherForecastElement::set_weather_type(WeatherType type)
{
    std::string text = GetTypeName(type);
    symbol_text->set_text(text.c_str());

    int referenceTextAmount = 12; //This amount of chars are a good fit with 0.0f offset
    int locationAmount = text.length();

    int diff = referenceTextAmount - locationAmount;
    symbol_text->set_size_offset((float)diff * 0.1f);

    lv_img_set_src(symbol, GetWeatherSymbol(type));
}

void WeatherForecastElement::set_position(float x, float y)
{
    float xSize = lv_obj_get_content_width(tile); //Size of x in pixels 
    float ySize = lv_obj_get_content_height(tile); //Size of y in pixels

    //Symbol
    lv_obj_set_pos(symbol, (x - 0.12 * size)*xSize, y*ySize); //Centered

    //Temp
    temperature_text->set_position((x + 0.12 * size) *xSize, y*ySize);

    //Location
    location_text->set_position(x*xSize, (y + 0.20f * size)*ySize); //Above

    //Time
    time_text->set_position(x*xSize, (y + 0.28f * size)*ySize); //Above

    //Symbol text
    symbol_text->set_position(x*xSize, (y - 0.20f * size)*ySize); //Above

    xPos = x;
    yPos = y;
}

void WeatherForecastElement::set_size(float size) //In percentage
{
    float finalSize = lv_obj_get_content_width(tile) * size;

    //Symbol
    float zoomPercentage = (finalSize / lv_obj_get_content_width(tile)) * 0.4f;
    lv_img_set_zoom(symbol, 256 * zoomPercentage);

    //Size intervals for the text size:
    if(this->size != size)
    {
        temperature_text->set_size(size);
        symbol_text->set_size(size);
        location_text->set_size(size);
        time_text->set_size(size);
            
        //Do this to update position relative to screen
        this->size = size;
        set_position(xPos, yPos);
    }
}
