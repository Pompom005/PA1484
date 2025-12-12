#pragma once
#include "SMHITestData.h"
#include "ParserStuff/SMHIForecastParser.h"
#include "ParserStuff/SMHIHistoricalParser.h"

class SMHITestRunner {
private:
    SMHIForecastParser forecastParser;
    SMHIHistoricalParser historicalParser;

public:
    void run_forecast_test(String testJSON);
    void run_historical_test();
    void run_all_tests();
    void run_timestamp_debug();
    void run_file_system_test();
    void debug_json_structure();
    void debug_timestamp_conversion();
    void debug_data_types();
    
private:
    void print_test_result(const String& testName, bool passed);
    bool compare_vectors(const std::vector<float>& actual, const std::vector<float>& expected);
};