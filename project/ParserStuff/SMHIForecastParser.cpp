#include "SMHIForecastParser.h"
#include <FS.h>
#include <SPIFFS.h>

SMHIForecastParser::SMHIForecastParser() {
}

SMHIForecastParser::~SMHIForecastParser() {
    clearData();
}

bool SMHIForecastParser::parseDateTime(const String& dateTimeStr, int& year, int& month, int& day) {
    if (dateTimeStr.length() < 10) return false;
    
    year = dateTimeStr.substring(0, 4).toInt();
    month = dateTimeStr.substring(5, 7).toInt();
    day = dateTimeStr.substring(8, 10).toInt();
    
    return true;
}

bool SMHIForecastParser::isNoonTime(const String& dateTimeStr) {
    // Check if time is 12:00 (format: "2025-11-29T12:00:00Z")
    if (dateTimeStr.length() < 16) return false;
    
    int hour = dateTimeStr.substring(11, 13).toInt();
    return hour == 12;
}

void SMHIForecastParser::extractForecastData(const String& jsonString) {
    clearData();
    
    const size_t capacity = JSON_ARRAY_SIZE(100) + 100 * JSON_OBJECT_SIZE(3) + 100 * JSON_OBJECT_SIZE(20) + 20000;
    DynamicJsonDocument doc(capacity);
    
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.print("Forecast JSON deserialization failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    JsonArray timeSeries = doc["timeSeries"];
    if (timeSeries.isNull()) {
        Serial.println("No timeSeries found in forecast JSON");
        return;
    }
    
    for (JsonObject dataPoint : timeSeries) {
        const char* timeStr = dataPoint["time"];
        JsonObject data = dataPoint["data"];
        
        if (timeStr && !data.isNull() && isNoonTime(String(timeStr))) {
            ForecastDataPoint point;
            
            if (parseDateTime(String(timeStr), point.year, point.month, point.day)) {
                point.temperature = data["air_temperature"] | -999.0f;
                point.windSpeed = data["wind_speed"] | -999.0f;
                point.humidity = data["relative_humidity"] | -999.0f;
                point.pressure = data["air_pressure_at_mean_sea_level"] | -999.0f;
                point.precipitation = data["precipitation_amount_mean"] | -999.0f;
                point.weatherSymbol = data["symbol_code"] | 0;
                
                forecastData.push_back(point);
            }
        }
    }
    
    Serial.print("Parsed ");
    Serial.print(forecastData.size());
    Serial.println(" forecast data points (12:00 only)");
}

bool SMHIForecastParser::parseJSONFromFile(const String& filename) {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return false;
    }
    
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open forecast file for reading");
        return false;
    }
    
    String jsonString = file.readString();
    file.close();
    
    extractForecastData(jsonString);
    
    // Delete file to save space
    SPIFFS.remove(filename);
    
    return forecastData.size() > 0;
}

bool SMHIForecastParser::parseJSONFromString(const String& jsonString) {
    extractForecastData(jsonString);
    return forecastData.size() > 0;
}

std::vector<float> SMHIForecastParser::getTemperatureData() const {
    std::vector<float> temperatures;
    for (const auto& data : forecastData) {
        if (data.temperature > -900.0f) {
            temperatures.push_back(data.temperature);
        }
    }
    return temperatures;
}

std::vector<float> SMHIForecastParser::getWindSpeedData() const {
    std::vector<float> windSpeeds;
    for (const auto& data : forecastData) {
        if (data.windSpeed > -900.0f) {
            windSpeeds.push_back(data.windSpeed);
        }
    }
    return windSpeeds;
}

std::vector<float> SMHIForecastParser::getHumidityData() const {
    std::vector<float> humidities;
    for (const auto& data : forecastData) {
        if (data.humidity > -900.0f) {
            humidities.push_back(data.humidity);
        }
    }
    return humidities;
}

std::vector<float> SMHIForecastParser::getPressureData() const {
    std::vector<float> pressures;
    for (const auto& data : forecastData) {
        if (data.pressure > -900.0f) {
            pressures.push_back(data.pressure);
        }
    }
    return pressures;
}

std::vector<float> SMHIForecastParser::getPrecipitationData() const {
    std::vector<float> precipitations;
    for (const auto& data : forecastData) {
        if (data.precipitation > -900.0f) {
            precipitations.push_back(data.precipitation);
        }
    }
    return precipitations;
}

void SMHIForecastParser::clearData() {
    forecastData.clear();
}

size_t SMHIForecastParser::getDataCount() const {
    return forecastData.size();
}

void SMHIForecastParser::printData() const {
    Serial.println("=== Forecast Data (12:00 only) ===");
    for (size_t i = 0; i < forecastData.size(); i++) {
        const auto& data = forecastData[i];
        Serial.printf("[%zu] %04d-%02d-%02d 12:00 - Temp: %.1f°C, Wind: %.1fm/s, Hum: %.0f%%, Press: %.1fhPa\n",
                     i, data.year, data.month, data.day,
                     data.temperature, data.windSpeed, data.humidity, data.pressure);
    }
}