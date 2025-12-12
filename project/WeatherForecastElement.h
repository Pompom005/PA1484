#pragma once
#include <string>
#include "WeatherType.h"
#include "LV_Helper.h"
#include "ScaleableLabel.h"

class WeatherForecastElement
{
    private:
    lv_obj_t* tile;
    
    //Created elements
    ScaleableLabel* temperature_text; //Temperature
    ScaleableLabel* location_text; //Location
    ScaleableLabel* time_text; //Time (maybe date?)
    ScaleableLabel* symbol_text;//Description of symbol (rain/sunny/cloudy etc)
    lv_obj_t* symbol;//Image

    //Used for setting pos/size better
    float size;
    float xPos; //Percentages
    float yPos;

    void create_texts_and_symbol();
    void set_default_values();

    public:

    WeatherForecastElement(lv_obj_t* parent_tile, float size = 1.0f);

    //Function for setting values on everything
    //Function for setting position, size etc. Should align stuff in a nice manner
    void set_values(float temperature, const std::string& location, const std::string& time, WeatherType type);
    void set_temp(float temperature);
    void set_location(const std::string& location);
    void set_time(const std::string& time);
    void set_weather_type(WeatherType type);

    void set_position(float x, float y); //In percentage on parent tile, 0,0 is middle.
    void set_size(float size); //In percentage to parent tile width, always square for now.
};

//Maybe eventually add refresh button to forecast screenf