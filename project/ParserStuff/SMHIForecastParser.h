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
    float wind_speed;
    float humidity;
    float pressure;
    float precipitation;
    int weather_symbol;
};

class SMHIForecastParser {
private:
    std::vector<ForecastDataPoint> forecast_data;
    
    bool parse_date_time(const String& dateTimeStr, int& year, int& month, int& day);
    bool is_noon_time(const String& dateTimeStr);
    void extract_forecast_data(const String& jsonString);

public:
    SMHIForecastParser();
    ~SMHIForecastParser();
    
    //Call this to fill with data
    bool parse_json_from_file(const String& filename);
    bool parse_json_from_string(const String& jsonString);
    bool get_data_from_json(JsonDocument& doc);
    
    //Call these to get data later
    std::vector<float> get_temperature_data() const;
    std::vector<float> get_wind_speed_data() const;
    std::vector<float> get_humidity_data() const;
    std::vector<float> get_pressure_data() const;
    std::vector<float> get_precipitation_data() const;
    
    const std::vector<ForecastDataPoint>& get_all_data() const;
    
    void clear_data();
    size_t get_data_count() const;
    
    void print_data() const;
};