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
    CreateTextsAndSymbol();
    SetDefaultValues(); //Just sets random 0 values.  
}

void WeatherForecastElement::CreateTextsAndSymbol()
{ 
    //Order matters for rendering order.
    //Symbol
    symbol = lv_img_create(tile);
    lv_obj_align(symbol, LV_ALIGN_CENTER, 0, 0);

    //Temperature
    temperature_text = new ScaleableLabel(tile, 1.0f, 0.6f);
    temperature_text->SetSize(size);

    //Location
    location_text = new ScaleableLabel(tile, 1.0f, -0.2f);
    location_text->SetSize(size);

    //Time
    time_text = new ScaleableLabel(tile, 1.0f, -0.2f);
    time_text->SetSize(size);

    //Symbol text
    symbol_text = new ScaleableLabel(tile);
    symbol_text->SetSize(size);
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
    temperature_text->SetText((stream.str() + "°").c_str());
}

void WeatherForecastElement::SetLocation(const std::string &location)
{
    location_text->SetText(location.c_str());
    int referenceTextAmount = 12; //This amount of chars are a good fit with 0.0f offset
    int locationAmount = location.length();

    int diff = referenceTextAmount - locationAmount;
    location_text->SetSizeOffset((float)diff * 0.1f);
}

void WeatherForecastElement::SetTime(const std::string &time)
{
    time_text->SetText(time.c_str());
}

void WeatherForecastElement::SetWeatherType(WeatherType type)
{
    std::string text = GetTypeName(type);
    symbol_text->SetText(text.c_str());

    int referenceTextAmount = 12; //This amount of chars are a good fit with 0.0f offset
    int locationAmount = text.length();

    int diff = referenceTextAmount - locationAmount;
    symbol_text->SetSizeOffset((float)diff * 0.1f);

    lv_img_set_src(symbol, GetWeatherSymbol(type));
}

void WeatherForecastElement::SetPosition(float x, float y)
{
    float xSize = lv_obj_get_content_width(tile); //Size of x in pixels 
    float ySize = lv_obj_get_content_height(tile); //Size of y in pixels

    //Symbol
    lv_obj_set_pos(symbol, (x - 0.12 * size)*xSize, y*ySize); //Centered

    //Temp
    temperature_text->SetPosition((x + 0.12 * size) *xSize, y*ySize);

    //Location
    location_text->SetPosition(x*xSize, (y + 0.20f * size)*ySize); //Above

    //Time
    time_text->SetPosition(x*xSize, (y + 0.28f * size)*ySize); //Above

    //Symbol text
    symbol_text->SetPosition(x*xSize, (y - 0.20f * size)*ySize); //Above

    xPos = x;
    yPos = y;
}

void WeatherForecastElement::SetSize(float size) //In percentage
{
    float finalSize = lv_obj_get_content_width(tile) * size;

    //Symbol
    float zoomPercentage = (finalSize / lv_obj_get_content_width(tile)) * 0.4f;
    lv_img_set_zoom(symbol, 256 * zoomPercentage);

    //Size intervals for the text size:
    if(this->size != size)
    {
        temperature_text->SetSize(size);
        symbol_text->SetSize(size);
        location_text->SetSize(size);
        time_text->SetSize(size);
            
        //Do this to update position relative to screen
        this->size = size;
        SetPosition(xPos, yPos);
    }
}
