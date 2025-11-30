#pragma once
#include <Arduino.h>
#include <vector>
#include <String>

class SMHITestData {
public:
    // Get hardcoded forecast JSON for testing
    static String getForecastTestJSON();
    
    // Get hardcoded historical data JSON for testing  
    static String getHistoricalTemperatureTestJSON();
    static String getHistoricalHumidityTestJSON();
    static String getHistoricalPressureTestJSON();
    static String getHistoricalWindTestJSON();
    
    // Get expected results for verification
    static std::vector<float> getExpectedTemperatures();
    static std::vector<float> getExpectedHumidity();
    static std::vector<float> getExpectedWindSpeeds();
    
    // Debug methods
    static void debugTimestamps();
    static void debugJSONStructure();
};