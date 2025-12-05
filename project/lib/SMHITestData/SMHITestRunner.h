#pragma once
#include "SMHITestData.h"
#include "ParserStuff/SMHIForecastParser.h"
#include "ParserStuff/SMHIHistoricalParser.h"

class SMHITestRunner {
private:
    SMHIForecastParser forecastParser;
    SMHIHistoricalParser historicalParser;

public:
    void runForecastTest(String testJSON);
    void runHistoricalTest();
    void runAllTests();
    void runTimestampDebug();
    void runFileSystemTest();
    void debugJSONStructure();
    void debugTimestampConversion();
    void debugDataTypes();
    
private:
    void printTestResult(const String& testName, bool passed);
    bool compareVectors(const std::vector<float>& actual, const std::vector<float>& expected);
};