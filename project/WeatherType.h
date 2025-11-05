#pragma once
#include <string>

enum class WeatherType
{
    Sunny,
    Cloudy,
    Rain,
    Snow,
    Thunder,
    Count
};

static std::string GetTypeName(WeatherType type)
{
    switch (type)
    {
    case WeatherType::Sunny:
        return "Sunny";
    
    case WeatherType::Cloudy:
        return "Cloudy";

    case WeatherType::Rain:
        return "Rainy";

    case WeatherType::Snow:
        return "Snowing";

    case WeatherType::Thunder:
        return "Thunder";

    default:
        return "No type";
    }
}