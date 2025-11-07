#include "WeatherAPI.h"
// include whatever HTTP + JSON library you have

WeatherAPI::WeatherAPI(const std::string& url)
    : baseUrl(url) {}

std::string WeatherAPI::fetchRawData(const std::string& stationId) {
    // Example placeholder using your library:
    // auto response = HTTP_GET(baseUrl + "/station/" + stationId + "/latest.json");
    // return response.body;
    return "{}"; // stub for now
}

WeatherData WeatherAPI::parseWeatherData(const std::string& jsonData) {
    WeatherData data;
    // next step: parse JSON into WeatherData
    return data;
}
