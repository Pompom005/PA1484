#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <String>
#include "LV_Helper.h"

struct HistoricalDataPoint {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    float value;
    String quality;
};

class SMHIHistoricalParser {
private:
    std::vector<HistoricalDataPoint> historical_data;
    String parameter_name;
    String unit;
    
    void timestamp_to_date_time(unsigned long timestamp, int& year, int& month, int& day, int& hour, int& minute);
    void extract_historical_data(const String& jsonString);

public:
    SMHIHistoricalParser();
    ~SMHIHistoricalParser();
    
    //Call this to fill with data
    bool parse_json_from_file(const String& filename);
    bool parse_json_from_string(const String& jsonString);
    bool get_data_from_json(JsonDocument& doc);
    
    //Call these to get data
    std::vector<lv_coord_t> get_value_data() const;
    const std::vector<HistoricalDataPoint>& get_all_data() const { return historical_data; }
    String get_parameter_name() const { return parameter_name; }
    String get_unit() const { return unit; }
    
    void clear_data();
    size_t get_data_count() const;
    
    void print_data() const;
};