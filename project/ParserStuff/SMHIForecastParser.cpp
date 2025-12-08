#include "SMHIForecastParser.h"
#include <LittleFS.h>


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
        return;
    }
    
    JsonArray timeSeries = doc["timeSeries"];
    if (timeSeries.isNull()) {
        return;
    }
    
    for (JsonObject dataPoint : timeSeries) {
        const char* timeStr = dataPoint["time"];
        JsonObject data = dataPoint["data"];
        
        if (timeStr && !data.isNull() && isNoonTime(String(timeStr))) {
            ForecastDataPoint point;
            
            if (parseDateTime(String(timeStr), point.year, point.month, point.day)) {
                // Handle both string and number types for all data fields
                point.temperature = data["air_temperature"].is<const char*>() ? 
                    String(data["air_temperature"].as<const char*>()).toFloat() : 
                    data["air_temperature"].as<float>();
                    
                point.windSpeed = data["wind_speed"].is<const char*>() ? 
                    String(data["wind_speed"].as<const char*>()).toFloat() : 
                    data["wind_speed"].as<float>();
                    
                point.humidity = data["relative_humidity"].is<const char*>() ? 
                    String(data["relative_humidity"].as<const char*>()).toFloat() : 
                    data["relative_humidity"].as<float>();
                    
                point.pressure = data["air_pressure_at_mean_sea_level"].is<const char*>() ? 
                    String(data["air_pressure_at_mean_sea_level"].as<const char*>()).toFloat() : 
                    data["air_pressure_at_mean_sea_level"].as<float>();
                    
                point.precipitation = data["precipitation_amount_mean"].is<const char*>() ? 
                    String(data["precipitation_amount_mean"].as<const char*>()).toFloat() : 
                    data["precipitation_amount_mean"].as<float>();
                    
                point.weatherSymbol = data["symbol_code"].is<const char*>() ? 
                    String(data["symbol_code"].as<const char*>()).toInt() : 
                    data["symbol_code"].as<int>();
                
                forecastData.push_back(point);
            }
        }
    }
}

bool SMHIForecastParser::parseJSONFromFile(const String& filename) {
    if (LittleFS.begin(true)) {
        File file = LittleFS.open(filename, "r");
        if (file) {
            String jsonString = file.readString();
            file.close();
            extractForecastData(jsonString);
            return forecastData.size() > 0;
        }
    }
    return false;
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