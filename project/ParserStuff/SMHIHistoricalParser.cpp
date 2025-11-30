#include "SMHIHistoricalParser.h"
#include <LittleFS.h>

SMHIHistoricalParser::SMHIHistoricalParser() : parameterName(""), unit("") {
}

SMHIHistoricalParser::~SMHIHistoricalParser() {
    clearData();
}

void SMHIHistoricalParser::timestampToDateTime(unsigned long timestamp, int& year, int& month, int& day, int& hour, int& minute) {
    // SMHI timestamps are milliseconds since 1900
    // Convert to seconds since 1970
    unsigned long offset = 2208988800000UL; // 70 years in milliseconds
    unsigned long millisecondsSince1970 = timestamp - offset;
    time_t rawTime = millisecondsSince1970 / 1000;
    
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
        return;
    }
    
    for (JsonObject dataPoint : values) {
        // Handle both string and number types for all fields
        unsigned long timestamp = 0;
        float value = 0.0f;
        const char* quality = nullptr;
        
        // Date can be string or number
        if (dataPoint["date"].is<const char*>()) {
            timestamp = String(dataPoint["date"].as<const char*>()).toInt();
        } else {
            timestamp = dataPoint["date"].as<unsigned long>();
        }
        
        // Value can be string or number  
        if (dataPoint["value"].is<const char*>()) {
            value = String(dataPoint["value"].as<const char*>()).toFloat();
        } else {
            value = dataPoint["value"].as<float>();
        }
        
        // Quality is always string
        quality = dataPoint["quality"].as<const char*>();
        
        if (quality && (String(quality) == "Y" || String(quality) == "G")) {
            HistoricalDataPoint point;
            
            timestampToDateTime(timestamp, point.year, point.month, point.day, point.hour, point.minute);
            point.value = value;
            point.quality = String(quality);
            
            historicalData.push_back(point);
        }
    }
}

bool SMHIHistoricalParser::parseJSONFromFile(const String& filename) {
    if (LittleFS.begin(true)) {
        File file = LittleFS.open(filename, "r");
        if (file) {
            String jsonString = file.readString();
            file.close();
            extractHistoricalData(jsonString);
            return historicalData.size() > 0;
        }
    }
    return false;
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
    
    size_t printCount = min(historicalData.size(), (size_t)10);
    for (size_t i = 0; i < printCount; i++) {
        const auto& data = historicalData[i];
        Serial.printf("[%zu] %04d-%02d-%02d %02d:%02d - Value: %.1f%s, Quality: %s\n",
                     i, data.year, data.month, data.day, data.hour, data.minute,
                     data.value, unit.c_str(), data.quality.c_str());
    }
    
    if (historicalData.size() > 10) {
        Serial.printf("... and %zu more data points\n", historicalData.size() - 10);
    }
}