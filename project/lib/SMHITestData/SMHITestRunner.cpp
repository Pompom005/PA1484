#include "SMHITestRunner.h"
#include <LittleFS.h>

void SMHITestRunner::runForecastTest(String testJSON) {
    Serial.println("\n" + String(50, '='));
    Serial.println("=== FORECAST PARSER TEST ===");
    Serial.println(String(50, '='));
    

    Serial.println(testJSON);
    if (forecastParser.parseJSONFromString(testJSON)) {
        Serial.println("✓ JSON parsing successful");
        forecastParser.printData();
        
        // Verify results
        std::vector<float> actualTemps = forecastParser.getTemperatureData();
        std::vector<float> expectedTemps = SMHITestData::getExpectedTemperatures();
        
        std::vector<float> actualHumidity = forecastParser.getHumidityData();
        std::vector<float> expectedHumidity = SMHITestData::getExpectedHumidity();
        
        std::vector<float> actualWind = forecastParser.getWindSpeedData();
        std::vector<float> expectedWind = SMHITestData::getExpectedWindSpeeds();
        
        bool tempTest = compareVectors(actualTemps, expectedTemps);
        bool humidityTest = compareVectors(actualHumidity, expectedHumidity);
        bool windTest = compareVectors(actualWind, expectedWind);
        
        printTestResult("Temperature parsing", tempTest);
        printTestResult("Humidity parsing", humidityTest);
        printTestResult("Wind speed parsing", windTest);
        
        if (tempTest && humidityTest && windTest) {
            Serial.println("\n🎉 ALL FORECAST TESTS PASSED!");
        } else {
            Serial.println("\n❌ SOME FORECAST TESTS FAILED!");
        }
        
    } else {
        Serial.println("❌ JSON parsing failed");
    }
}

void SMHITestRunner::runHistoricalTest() {
    Serial.println("\n" + String(50, '='));
    Serial.println("=== HISTORICAL PARSER TEST ===");
    Serial.println(String(50, '='));
    
    // Test temperature historical data
    String tempJSON = SMHITestData::getHistoricalTemperatureTestJSON();
    if (historicalParser.parseJSONFromString(tempJSON)) {
        Serial.println("✓ Historical temperature parsing successful");
        historicalParser.printData();
        
        std::vector<lv_coord_t> values = historicalParser.getValueData();
        Serial.printf("Parsed %d historical data points\n", values.size());
        Serial.print("Values: ");
        for (float val : values) {
            Serial.printf("%.1f ", val);
        }
        Serial.println();
        
        printTestResult("Historical temperature parsing", values.size() == 17);
    } else {
        Serial.println("❌ Historical temperature parsing failed");
    }
    
    Serial.println();
    
    // Test humidity historical data
    String humidityJSON = SMHITestData::getHistoricalHumidityTestJSON();
    if (historicalParser.parseJSONFromString(humidityJSON)) {
        Serial.println("✓ Historical humidity parsing successful");
        Serial.printf("Parameter: %s\n", historicalParser.getParameterName().c_str());
        Serial.printf("Unit: %s\n", historicalParser.getUnit().c_str());
        
        std::vector<lv_coord_t> values = historicalParser.getValueData();
        Serial.printf("Parsed %d humidity data points\n", values.size());
        
        printTestResult("Historical humidity parsing", values.size() == 7);
    } else {
        Serial.println("❌ Historical humidity parsing failed");
    }
}

void SMHITestRunner::runAllTests() {
    Serial.println("\n" + String(60, '='));
    Serial.println("=== SMHI PARSER COMPLETE TEST SUITE ===");
    Serial.println(String(60, '='));
    
    runForecastTest(SMHITestData::getForecastTestJSON());
    runHistoricalTest();
    
    Serial.println("\n" + String(60, '='));
    Serial.println("=== TEST SUITE COMPLETE ===");
    Serial.println(String(60, '='));
}

void SMHITestRunner::runTimestampDebug() {
    Serial.println("\n" + String(50, '='));
    Serial.println("=== TIMESTAMP DEBUG ===");
    Serial.println(String(50, '='));
    
    SMHITestData::debugTimestamps();
    SMHITestData::debugJSONStructure();
}

void SMHITestRunner::runFileSystemTest() {
    Serial.println("\n" + String(50, '='));
    Serial.println("=== FILE SYSTEM TEST ===");
    Serial.println(String(50, '='));
    
    if (LittleFS.begin(true)) {
        Serial.println("✓ LittleFS mounted successfully");
        
        // List files
        File root = LittleFS.open("/");
        File file = root.openNextFile();
        int count = 0;
        
        Serial.println("Files in LittleFS:");
        while(file){
            Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
            count++;
            file = root.openNextFile();
        }
        
        if(count == 0) {
            Serial.println("  No files found");
        }
    } else {
        Serial.println("❌ LittleFS mount failed");
    }
}

void SMHITestRunner::debugJSONStructure() {
    Serial.println("\n=== JSON STRUCTURE DEBUG ===");
    
    SMHIHistoricalParser parser;
    String testJSON = SMHITestData::getHistoricalTemperatureTestJSON();
    
    const size_t capacity = 10000;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, testJSON);
    
    if (!error) {
        if (doc.containsKey("value")) {
            JsonArray values = doc["value"];
            Serial.printf("Found 'value' array with %d elements\n", values.size());
            
            if (values.size() > 0) {
                JsonObject first = values[0];
                Serial.println("First object fields:");
                for (JsonPair kv : first) {
                    Serial.printf("  %s: ", kv.key().c_str());
                    if (kv.value().is<unsigned long>()) {
                        Serial.printf("%lu (number)\n", kv.value().as<unsigned long>());
                    } else if (kv.value().is<const char*>()) {
                        Serial.printf("%s (string)\n", kv.value().as<const char*>());
                    } else {
                        Serial.printf("(other type)\n");
                    }
                }
                
                // Specifically check the date field
                if (first.containsKey("date")) {
                    unsigned long testTimestamp = 0;
                    if (first["date"].is<const char*>()) {
                        testTimestamp = String(first["date"].as<const char*>()).toInt();
                    } else {
                        testTimestamp = first["date"].as<unsigned long>();
                    }
                    Serial.printf("DEBUG: First timestamp value: %lu\n", testTimestamp);
                } else {
                    Serial.println("DEBUG: No 'date' field found!");
                }
            }
        } else {
            Serial.println("No 'value' array found!");
        }
    } else {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
    }
}

void SMHITestRunner::debugTimestampConversion() {
    Serial.println("\n=== TIMESTAMP CONVERSION DEBUG ===");
    
    // Test specific timestamps from your data
unsigned long long testTimestamps[] = {1753146000000ULL, 1753149600000ULL, 1753153200000ULL};
    
    for (auto timestamp : testTimestamps) {
        Serial.printf("Testing timestamp: %lu\n", timestamp);
        
        // Try different conversions
        unsigned long offset = 2208988800000UL; // 70 years in milliseconds
        unsigned long millisecondsSince1970 = timestamp - offset;
        time_t rawTime = millisecondsSince1970 / 1000;
        
        struct tm* timeInfo = gmtime(&rawTime);
        Serial.printf("  Converted to: %04d-%02d-%02d %02d:%02d\n", 
                     timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, 
                     timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
    }
}

void SMHITestRunner::debugDataTypes() {
    Serial.println("\n=== DATA TYPES DEBUG ===");
    
    String testJSON = SMHITestData::getHistoricalTemperatureTestJSON();
    
    const size_t capacity = 10000;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, testJSON);
    
    if (!error) {
        JsonArray values = doc["value"];
        if (values.size() > 0) {
            JsonObject first = values[0];
            Serial.println("First object field types:");
            for (JsonPair kv : first) {
                Serial.printf("  %s: ", kv.key().c_str());
                if (kv.value().is<unsigned long>()) {
                    Serial.printf("number (%lu)\n", kv.value().as<unsigned long>());
                } else if (kv.value().is<const char*>()) {
                    Serial.printf("string (%s)\n", kv.value().as<const char*>());
                } else if (kv.value().is<float>()) {
                    Serial.printf("float (%.1f)\n", kv.value().as<float>());
                } else {
                    Serial.printf("other type\n");
                }
            }
        }
    }
}

void SMHITestRunner::printTestResult(const String& testName, bool passed) {
    Serial.printf("  %s: %s\n", testName.c_str(), passed ? "✓ PASS" : "✗ FAIL");
}

bool SMHITestRunner::compareVectors(const std::vector<float>& actual, const std::vector<float>& expected) {
    if (actual.size() != expected.size()) {
        Serial.printf("    Size mismatch: expected %zu, got %zu\n", expected.size(), actual.size());
        return false;
    }
    
    for (size_t i = 0; i < actual.size(); i++) {
        if (abs(actual[i] - expected[i]) > 0.01) {
            Serial.printf("    Value mismatch at index %zu: expected %.1f, got %.1f\n", 
                         i, expected[i], actual[i]);
            return false;
        }
    }
    
    return true;
}