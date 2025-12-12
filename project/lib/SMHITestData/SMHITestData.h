#pragma once
#include <Arduino.h>
#include <vector>
#include <String>

class SMHITestData {
public:
    // Get hardcoded forecast JSON for testing
    static String get_forecast_test_json();
    
    // Get hardcoded historical data JSON for testing  
    static String get_historical_temperature_test_json();
    static String get_historical_humidity_test_json();
    static String get_historical_pressure_test_json();
    static String get_historical_wind_test_json();
    
    // Get expected results for verification
    static std::vector<float> get_expected_temperature();
    static std::vector<float> get_expected_humidity();
    static std::vector<float> get_expected_wind_speeds();
    
    // Debug methods
    static void debug_timestamps();
    static void debug_json_structure();
};