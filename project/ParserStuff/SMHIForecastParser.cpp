#include "SMHIForecastParser.h"
#include <LittleFS.h>
#include <sstream>


SMHIForecastParser::SMHIForecastParser() {
}

SMHIForecastParser::~SMHIForecastParser() {
    clear_data();
}

bool SMHIForecastParser::parse_date_time(const String& dateTimeStr, int& year, int& month, int& day) {
    if (dateTimeStr.length() < 10) return false;
    
    year = dateTimeStr.substring(0, 4).toInt();
    month = dateTimeStr.substring(5, 7).toInt();
    day = dateTimeStr.substring(8, 10).toInt();
    
    return true;
}

bool SMHIForecastParser::is_noon_time(const String& dateTimeStr) {
    if (dateTimeStr.length() < 16) return false;
    
    int hour = dateTimeStr.substring(11, 13).toInt();
    return hour == 12;
}

void SMHIForecastParser::extract_forecast_data(const String& jsonString) {
    clear_data();
    
    const size_t capacity = JSON_ARRAY_SIZE(100) + 100 * JSON_OBJECT_SIZE(3) + 100 * JSON_OBJECT_SIZE(20) + 20000;
    DynamicJsonDocument doc(capacity);
    
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        return;
    }
    Serial.println(error.c_str());

    JsonArray timeSeries = doc["timeSeries"].as<JsonArray>();
    if (timeSeries.isNull()) {
        return;
    }
    Serial.print("Timeseries exists");
    for (JsonObject dataPoint : timeSeries) {
        const char* timeStr = dataPoint["time"];
        JsonObject data = dataPoint["data"];
        
        if (timeStr && !data.isNull() && is_noon_time(String(timeStr))) {
            ForecastDataPoint point;
            
            if (parse_date_time(String(timeStr), point.year, point.month, point.day)) {
                // Handle both string and number types for all data fields
                point.temperature = data["air_temperature"].is<const char*>() ? 
                    String(data["air_temperature"].as<const char*>()).toFloat() : 
                    data["air_temperature"].as<float>();
                    
                point.wind_speed = data["wind_speed"].is<const char*>() ? 
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
                    
                point.weather_symbol = data["symbol_code"].is<const char*>() ? 
                    String(data["symbol_code"].as<const char*>()).toInt() : 
                    data["symbol_code"].as<int>();
                
                forecast_data.push_back(point);
            }
        }
    }
}

bool SMHIForecastParser::parse_json_from_file(const String& filename) {
    if (LittleFS.begin(true)) {
        File file = LittleFS.open(filename, "r");
        if (file) {
            String jsonString = file.readString();
            file.close();
            extract_forecast_data(jsonString);
            return forecast_data.size() > 0;
        }
    }
    return false;
}

bool SMHIForecastParser::parse_json_from_string(const String& jsonString) {
    extract_forecast_data(jsonString);
    return forecast_data.size() > 0;
}

bool SMHIForecastParser::get_data_from_json(JsonDocument &doc)
{
    JsonArray timeSeries = doc["timeSeries"];
    if (timeSeries.isNull()) {
        return false;
    }
    
    for (JsonObject dataPoint : timeSeries) 
    {
        const char* timeStr = dataPoint["time"];
        JsonObject data = dataPoint["data"];
        
        if (timeStr && !data.isNull() && is_noon_time(String(timeStr))) {
            ForecastDataPoint point;
            
            if (parse_date_time(String(timeStr), point.year, point.month, point.day)) {
                // Handle both string and number types for all data fields
                point.temperature = data["air_temperature"].is<const char*>() ? 
                    String(data["air_temperature"].as<const char*>()).toFloat() : 
                    data["air_temperature"].as<float>();
                    
                point.wind_speed = data["wind_speed"].is<const char*>() ? 
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
                    
                point.weather_symbol = data["symbol_code"].is<const char*>() ? 
                    String(data["symbol_code"].as<const char*>()).toInt() : 
                    data["symbol_code"].as<int>();
                
                forecast_data.push_back(point);
            }
        }
    }
    return true;
}

std::vector<float> SMHIForecastParser::get_temperature_data() const {
    std::vector<float> temperatures;
    for (const auto& data : forecast_data) {
        if (data.temperature > -900.0f) {
            temperatures.push_back(data.temperature);
        }
    }
    return temperatures;
}

std::vector<float> SMHIForecastParser::get_wind_speed_data() const {
    std::vector<float> windSpeeds;
    for (const auto& data : forecast_data) {
        if (data.wind_speed > -900.0f) {
            windSpeeds.push_back(data.wind_speed);
        }
    }
    return windSpeeds;
}

std::vector<float> SMHIForecastParser::get_humidity_data() const {
    std::vector<float> humidities;
    for (const auto& data : forecast_data) {
        if (data.humidity > -900.0f) {
            humidities.push_back(data.humidity);
        }
    }
    return humidities;
}

std::vector<float> SMHIForecastParser::get_pressure_data() const {
    std::vector<float> pressures;
    for (const auto& data : forecast_data) {
        if (data.pressure > -900.0f) {
            pressures.push_back(data.pressure);
        }
    }
    return pressures;
}

std::vector<float> SMHIForecastParser::get_precipitation_data() const {
    std::vector<float> precipitations;
    for (const auto& data : forecast_data) {
        if (data.precipitation > -900.0f) {
            precipitations.push_back(data.precipitation);
        }
    }
    return precipitations;
}

const std::vector<ForecastDataPoint> &SMHIForecastParser::get_all_data() const
{
    return forecast_data;
}

void SMHIForecastParser::clear_data() {
    forecast_data.clear();
}

size_t SMHIForecastParser::get_data_count() const {
    return forecast_data.size();
}

void SMHIForecastParser::print_data() const {
    Serial.println("=== Forecast Data (12:00 only) ===");
    for (size_t i = 0; i < forecast_data.size(); i++) {
        const auto& data = forecast_data[i];
        Serial.printf("[%zu] %04d-%02d-%02d 12:00 - Temp: %.1f°C, Wind: %.1fm/s, Hum: %.0f%%, Press: %.1fhPa\n",
                     i, data.year, data.month, data.day,
                     data.temperature, data.wind_speed, data.humidity, data.pressure);
    }
}
