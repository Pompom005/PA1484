#include "DataHandler.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>

// CSV handling implementation
bool DataHandler::loadAndParseCSV(const std::string& filename, char delimiter) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    bool result = parseCSVString(buffer.str(), delimiter);
    
    // Delete the file to free space if parsing was successful
    if (result) {
        std::remove(filename.c_str());
    }
    
    return result;
}

bool DataHandler::parseCSVString(const std::string& csvContent, char delimiter) {
    clearCSVData();
    
    std::istringstream ss(csvContent);
    std::string line;
    
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        
        WeatherDataPoint dataPoint;
        if (parseCSVLine(line, delimiter, dataPoint)) {
            csvData.push_back(dataPoint);
        }
    }
    
    return !csvData.empty();
}

bool DataHandler::parseCSVLine(const std::string& line, char delimiter, WeatherDataPoint& result) {
    std::istringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 3) {
        return false;
    }
    
    // Parse datetime (format: "2025-11-12;08:00:00;9.1")
    std::string datetime = tokens[0];
    size_t spacePos = datetime.find(' ');
    
    if (spacePos != std::string::npos) {
        result.date = datetime.substr(0, spacePos);
        result.time = datetime.substr(spacePos + 1);
    } else {
        result.date = datetime;
        result.time = "00:00:00";
    }
    
    // Parse value
    try {
        result.value = std::stod(tokens[1]);
    } catch (...) {
        return false;
    }
    
    // Parse unit if available
    if (tokens.size() > 2) {
        result.unit = tokens[2];
    }
    
    return true;
}

// JSON handling implementation
bool DataHandler::loadAndParseJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    bool result = parseJSONString(buffer.str());
    
    // Delete the file to free space if parsing was successful
    if (result) {
        std::remove(filename.c_str());
    }
    
    return result;
}

bool DataHandler::parseJSONString(const std::string& jsonContent) {
    return parseForecastJSON(jsonContent);
}

bool DataHandler::parseForecastJSON(const std::string& jsonContent) {
    clearForecastData();
    
    forecastData.createdTime = extractJSONString(jsonContent, "createdTime");
    forecastData.referenceTime = extractJSONString(jsonContent, "referenceTime");
    
    // Parse time series
    size_t seriesStart = jsonContent.find("\"timeSeries\":[");
    if (seriesStart == std::string::npos) {
        return false;
    }
    
    seriesStart += 13;
    size_t seriesEnd = jsonContent.find("]", seriesStart);
    std::string seriesContent = jsonContent.substr(seriesStart, seriesEnd - seriesStart);
    
    size_t entryStart = 0;
    while ((entryStart = seriesContent.find("{", entryStart)) != std::string::npos) {
        size_t entryEnd = seriesContent.find("}", entryStart);
        if (entryEnd == std::string::npos) break;
        
        std::string entry = seriesContent.substr(entryStart, entryEnd - entryStart + 1);
        
        WeatherDataPoint dataPoint;
        dataPoint.time = extractJSONString(entry, "time");
        
        // Extract only temperature and symbol code as requested
        dataPoint.value = extractJSONDouble(entry, "air_temperature");
        dataPoint.symbol_code = extractJSONInt(entry, "symbol_code");
        
        forecastData.timeSeries.push_back(dataPoint);
        entryStart = entryEnd + 1;
    }
    
    return !forecastData.timeSeries.empty();
}

// JSON parsing helpers
std::string DataHandler::extractJSONString(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\":");
    if (pos == std::string::npos) return "";
    
    pos = json.find(":", pos);
    size_t start = json.find("\"", pos) + 1;
    size_t end = json.find("\"", start);
    return json.substr(start, end - start);
}

double DataHandler::extractJSONDouble(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\":");
    if (pos == std::string::npos) return 0.0;
    
    pos = json.find(":", pos);
    size_t start = pos + 1;
    size_t end = json.find_first_of(",}", start);
    
    std::string valueStr = json.substr(start, end - start);
    try {
        return std::stod(valueStr);
    } catch (...) {
        return 0.0;
    }
}

int DataHandler::extractJSONInt(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\":");
    if (pos == std::string::npos) return 0;
    
    pos = json.find(":", pos);
    size_t start = pos + 1;
    size_t end = json.find_first_of(",}", start);
    
    std::string valueStr = json.substr(start, end - start);
    try {
        return std::stoi(valueStr);
    } catch (...) {
        return 0;
    }
}

// Data extraction for graphs
std::vector<float> DataHandler::extractValuesForGraph() const {
    std::vector<float> values;
    values.reserve(csvData.size());
    
    for (const auto& dataPoint : csvData) {
        values.push_back(static_cast<float>(dataPoint.value));
    }
    
    return values;
}

std::vector<float> DataHandler::extractForecastTemperatures() const {
    std::vector<float> temperatures;
    temperatures.reserve(forecastData.timeSeries.size());
    
    for (const auto& dataPoint : forecastData.timeSeries) {
        temperatures.push_back(static_cast<float>(dataPoint.value));
    }
    
    return temperatures;
}

std::vector<std::pair<float, int>> DataHandler::extractForecastTemperaturesAndSymbols() const {
    std::vector<std::pair<float, int>> data;
    data.reserve(forecastData.timeSeries.size());
    
    for (const auto& dataPoint : forecastData.timeSeries) {
        data.emplace_back(static_cast<float>(dataPoint.value), dataPoint.symbol_code);
    }
    
    return data;
}

// Data management
void DataHandler::clearCSVData() {
    csvData.clear();
    csvData.shrink_to_fit();
}

void DataHandler::clearForecastData() {
    forecastData.timeSeries.clear();
    forecastData.timeSeries.shrink_to_fit();
    forecastData.createdTime.clear();
    forecastData.referenceTime.clear();
}

void DataHandler::clearAllData() {
    clearCSVData();
    clearForecastData();
}