#pragma once
#include <string>
#include "LV_Helper.h"

enum class WeatherType
{
    Clear = 1,
    NearlyClear,
    VariableCloudiness,
    Halfclear,
    Cloudy,
    Overcast,
    Fog,
    LightRainShowers,
    ModerateRainShowers,
    HeavyRainShowers,
    Thunderstorm,
    LightSleetShowers,
    ModerateSleetShowers,
    HeavySleetShowers,
    LightSnowShowers,
    ModerateSnowShowers,
    HeavySnowShowers,
    LightRain,
    ModerateRain,
    HeavyRain,
    Thunder,
    LightSleet,
    ModerateSleet,
    HeavySleet,
    LightSnow,
    ModerateSnow,
    HeavySnow,
    Count
};

//Cant have more because of memory issues
LV_IMG_DECLARE(Thunder);
LV_IMG_DECLARE(Thunderstorm);
LV_IMG_DECLARE(SleetShowers);
LV_IMG_DECLARE(Fog);
LV_IMG_DECLARE(HalfClear);
LV_IMG_DECLARE(Clear);
LV_IMG_DECLARE(Overcast);
LV_IMG_DECLARE(Rain);
LV_IMG_DECLARE(RainShowers);
LV_IMG_DECLARE(Sleet);
LV_IMG_DECLARE(SnowShowers);
LV_IMG_DECLARE(Snow);

static const lv_img_dsc_t* GetWeatherSymbol(WeatherType symbol_code)
{
    //Did this before I made enum aswell, and im lazy so yeah : )
    int code = static_cast<int>(symbol_code);
    switch (code)
    {
// 1	Clear sky
        case 1:
            return &Clear;
// 2	Nearly clear sky
        case 2:
            return &Clear;
// 3	Variable cloudiness
        case 3:
            return &HalfClear;
// 4	Halfclear sky
        case 4:
            return &HalfClear;
// 5	Cloudy sky
        case 5:
            return &Overcast;
// 6	Overcast
        case 6:
            return &Overcast;
// 7	Fog
        case 7:
            return &Fog;
// 8	Light rain showers
        case 8:
            return &RainShowers;
// 9	Moderate rain showers
        case 9:
            return &RainShowers;
// 10	Heavy rain showers
        case 10:
            return &RainShowers;
// 11	Thunderstorm
        case 11:
            return &Thunderstorm;
// 12	Light sleet showers
        case 12:
            return &SleetShowers;
// 13	Moderate sleet showers
        case 13:
            return &SleetShowers;
// 14	Heavy sleet showers
        case 14:
            return &SleetShowers;
// 15	Light snow showers
        case 15:
            return &SnowShowers;
// 16	Moderate snow showers
        case 16:
            return &SnowShowers;
// 17	Heavy snow showers
        case 17:
            return &SnowShowers;
// 18	Light rain
        case 18:
            return &Rain;
// 19	Moderate rain
        case 19:
            return &Rain;
// 20	Heavy rain
        case 20:
            return &Rain;
// 21	Thunder
        case 21:
            return &Thunder;
// 22	Light sleet
        case 22:
            return &Sleet;
// 23	Moderate sleet
        case 23:
            return &Sleet;
// 24	Heavy sleet
        case 24:
            return &Sleet;
// 25	Light snowfall
        case 25:
            return &Snow;
// 26	Moderate snowfall
        case 26:
            return &Snow;
// 27	Heavy snowfall
        case 27:
            return &Snow;
    default:
        return nullptr;
    }
}

static std::string GetTypeName(WeatherType type)
{
    switch (type)
    {
        // 1	Clear sky
        case WeatherType::Clear:
            return "Clear Sky";
// 2	Nearly clear sky
        case WeatherType::NearlyClear:
            return "Nearly Clear Sky";
// 3	Variable cloudiness
        case WeatherType::VariableCloudiness:
            return "Variable Cloudiness";
// 4	Halfclear sky
        case WeatherType::Halfclear:
            return "Halfclear Sky";
// 5	Cloudy sky
        case WeatherType::Cloudy:
            return "Cloudy Sky";
// 6	Overcast
        case WeatherType::Overcast:
            return "Overcast";
// 7	Fog
        case WeatherType::Fog:
            return "Fog";
// 8	Light rain showers
        case WeatherType::LightRainShowers:
            return "Light Rain Showers";
// 9	Moderate rain showers
        case WeatherType::ModerateRainShowers:
            return "Moderate Rain Showers";
// 10	Heavy rain showers
        case WeatherType::HeavyRainShowers:
            return "Heavy Rain Showers";
// 11	Thunderstorm
        case WeatherType::Thunderstorm:
            return "Thunderstorm";
// 12	Light sleet showers
        case WeatherType::LightSleetShowers:
            return "Light Sleet Showers";
// 13	Moderate sleet showers
        case WeatherType::ModerateSleetShowers:
            return "Moderate Sleet Showers";
// 14	Heavy sleet showers
        case WeatherType::HeavySleetShowers:
            return "Heavy Sleet Showers";
// 15	Light snow showers
        case WeatherType::LightSnowShowers:
            return "Light Snow Showers";
// 16	Moderate snow showers
        case WeatherType::ModerateSnowShowers:
            return "Moderate Snow Showers";
// 17	Heavy snow showers
        case WeatherType::HeavySnowShowers:
            return "Heavy Snow Showers";
// 18	Light rain
        case WeatherType::LightRain:
            return "Light Rain";
// 19	Moderate rain
        case WeatherType::ModerateRain:
            return "Moderate Rain";
// 20	Heavy rain
        case WeatherType::HeavyRain:
            return "Heavy Rain";
// 21	Thunder
        case WeatherType::Thunder:
            return "Thunder";
// 22	Light sleet
        case WeatherType::LightSleet:
            return "Light Sleet";
// 23	Moderate sleet
        case WeatherType::ModerateSleet:
            return "Moderate Sleet";
// 24	Heavy sleet
        case WeatherType::HeavySleet:
            return "Heavy Sleet";
// 25	Light snowfall
        case WeatherType::LightSnow:
            return "Light Snowfall";
// 26	Moderate snowfall
        case WeatherType::ModerateSnow:
            return "Moderate Snowfall";
// 27	Heavy snowfall
        case WeatherType::HeavySnow:
            return "Heavy Snowfall";
    default:
        return "No type";
    }
}