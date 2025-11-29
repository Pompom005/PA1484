#include "SMHIHistoricalParser.h"
#include <FS.h>
#include <SPIFFS.h>

SMHIHistoricalParser::SMHIHistoricalParser() : parameterName(""), unit("") {
}

SMHIHistoricalParser::~SMHIHistoricalParser() {
    clearData();
}

void SMHIHistoricalParser::timestampToDateTime(unsigned long timestamp, int& year, int& month, int& day, int& hour, int& minute) {
    // Convert UNIX timestamp (milliseconds) to date/time
    time_t rawTime = timestamp / 1000;
    struct tm* timeInfo = gmtime(&rawTime);
    
    year = timeInfo->tm_year + 1900;
    month = timeInfo->tm_mon + 1;
    day = timeInfo->tm_mday;
    hour = timeInfo->tm_hour;
    minute = timeInfo->tm_min;
}

void SMHIHistoricalParser::extractHistoricalData(const String& jsonString) {
    clearData();
    
    const size_t capacity = JSON_ARRAY_SIZE(1000) + 1000 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(10) + 30000;
    DynamicJsonDocument doc(capacity);
    
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.print("Historical JSON deserialization failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Extract metadata
    if (doc["parameter"].containsKey("name")) {
        parameterName = doc["parameter"]["name"].as<String>();
    }
    if (doc["parameter"].containsKey("unit")) {
        unit = doc["parameter"]["unit"].as<String>();
    }
    
    // Extract values array
    JsonArray values = doc["value"];
    if (values.isNull()) {
        Serial.println("No value array found in historical JSON");
        return;
    }
    
    for (JsonObject dataPoint : values) {
        unsigned long timestamp = dataPoint["date"];
        const char* valueStr = dataPoint["value"];
        const char* quality = dataPoint["quality"];
        
        if (valueStr && quality && String(quality) == "Y") { // Only use validated data
            HistoricalDataPoint point;
            
            timestampToDateTime(timestamp, point.year, point.month, point.day, point.hour, point.minute);
            point.value = String(valueStr).toFloat();
            point.quality = String(quality);
            
            historicalData.push_back(point);
        }
    }
    
    Serial.print("Parsed ");
    Serial.print(historicalData.size());
    Serial.print(" historical data points for ");
    Serial.println(parameterName);
}

bool SMHIHistoricalParser::parseJSONFromFile(const String& filename) {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return false;
    }
    
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open historical file for reading");
        return false;
    }
    
    String jsonString = file.readString();
    file.close();
    
    extractHistoricalData(jsonString);
    
    // Delete file to save space
    SPIFFS.remove(filename);
    
    return historicalData.size() > 0;
}

bool SMHIHistoricalParser::parseJSONFromString(const String& jsonString) {
    extractHistoricalData(jsonString);
    return historicalData.size() > 0;
}

std::vector<float> SMHIHistoricalParser::getValueData() const {
    std::vector<float> values;
    for (const auto& data : historicalData) {
        values.push_back(data.value);
    }
    return values;
}

void SMHIHistoricalParser::clearData() {
    historicalData.clear();
    parameterName = "";
    unit = "";
}

size_t SMHIHistoricalParser::getDataCount() const {
    return historicalData.size();
}

void SMHIHistoricalParser::printData() const {
    Serial.println("=== Historical Data ===");
    Serial.print("Parameter: ");
    Serial.println(parameterName);
    Serial.print("Unit: ");
    Serial.println(unit);
    
    // Print first 10 and last 10 data points
    size_t printCount = min(historicalData.size(), (size_t)20);
    for (size_t i = 0; i < printCount; i++) {
        const auto& data = historicalData[i];
        Serial.printf("[%zu] %04d-%02d-%02d %02d:%02d - Value: %.1f%s, Quality: %s\n",
                     i, data.year, data.month, data.day, data.hour, data.minute,
                     data.value, unit.c_str(), data.quality.c_str());
    }
    
    if (historicalData.size() > 20) {
        Serial.println("... (more data points)");
        
        for (size_t i = historicalData.size() - 10; i < historicalData.size(); i++) {
            const auto& data = historicalData[i];
            Serial.printf("[%zu] %04d-%02d-%02d %02d:%02d - Value: %.1f%s, Quality: %s\n",
                         i, data.year, data.month, data.day, data.hour, data.minute,
                         data.value, unit.c_str(), data.quality.c_str());
        }
    }
}