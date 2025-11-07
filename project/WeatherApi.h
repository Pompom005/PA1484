#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <string>
#include "WeatherData.h"

class WeatherAPI {
private:
    std::string baseUrl;

public:
    WeatherAPI(const std::string& url);
    std::string fetchRawData(const std::string& stationId); // returns JSON as string
    WeatherData parseWeatherData(const std::string& jsonData); // stub for next step
};

#endif
