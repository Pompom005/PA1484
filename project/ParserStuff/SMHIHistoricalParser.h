#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <String>

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
    std::vector<HistoricalDataPoint> historicalData;
    String parameterName;
    String unit;
    
    void timestampToDateTime(unsigned long timestamp, int& year, int& month, int& day, int& hour, int& minute);
    void extractHistoricalData(const String& jsonString);

public:
    SMHIHistoricalParser();
    ~SMHIHistoricalParser();
    
    bool parseJSONFromFile(const String& filename);
    bool parseJSONFromString(const String& jsonString);
    
    std::vector<float> getValueData() const;
    const std::vector<HistoricalDataPoint>& getAllData() const { return historicalData; }
    String getParameterName() const { return parameterName; }
    String getUnit() const { return unit; }
    
    void clearData();
    size_t getDataCount() const;
    
    void printData() const;
};