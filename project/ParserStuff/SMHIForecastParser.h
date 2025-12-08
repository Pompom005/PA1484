#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <String>
#include "SMHIStationsAndParameters/SMHIStation.h"

struct ForecastDataPoint {
    int year;
    int month;
    int day;
    float temperature;
    float windSpeed;
    float humidity;
    float pressure;
    float precipitation;
    int weatherSymbol;
};

class SMHIForecastParser {
private:
    std::vector<ForecastDataPoint> forecastData;
    
    bool parseDateTime(const String& dateTimeStr, int& year, int& month, int& day);
    bool isNoonTime(const String& dateTimeStr);
    void extractForecastData(const String& jsonString);

public:
    SMHIForecastParser();
    ~SMHIForecastParser();
    
    bool parseJSONFromFile(const String& filename);
    bool parseJSONFromString(const String& jsonString);
    bool getDataFromJSON(JsonDocument& doc);
    
    std::vector<float> getTemperatureData() const;
    std::vector<float> getWindSpeedData() const;
    std::vector<float> getHumidityData() const;
    std::vector<float> getPressureData() const;
    std::vector<float> getPrecipitationData() const;
    
    const std::vector<ForecastDataPoint>& getAllData() const;
    
    void clearData();
    size_t getDataCount() const;
    
    void printData() const;
};