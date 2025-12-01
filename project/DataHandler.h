#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <vector>
#include <string>
#include <utility>

// Unified data structure for both CSV and JSON data
struct WeatherDataPoint {
    std::string date;
    std::string time;
    double value;
    std::string unit;
    int symbol_code;  // For JSON forecast data
    
    WeatherDataPoint(const std::string& d = "", const std::string& t = "", 
                    double v = 0.0, const std::string& u = "", int symbol = 0)
        : date(d), time(t), value(v), unit(u), symbol_code(symbol) {}
};

// Simple forecast structure for JSON data
struct ForecastData {
    std::string createdTime;
    std::string referenceTime;
    std::vector<WeatherDataPoint> timeSeries;
};

class DataHandler {
private:
    std::vector<WeatherDataPoint> csvData;
    ForecastData forecastData;

public:
    // CSV handling methods
    bool loadAndParseCSV(const std::string& filename, char delimiter = ';');
    bool parseCSVString(const std::string& csvContent, char delimiter = ';');
    
    // JSON handling methods
    bool loadAndParseJSON(const std::string& filename);
    bool parseJSONString(const std::string& jsonContent);
    
    // Data access methods
    const std::vector<WeatherDataPoint>& getCSVData() const { return csvData; }
    const ForecastData& getForecastData() const { return forecastData; }
    const std::vector<WeatherDataPoint>& getForecastSeries() const { return forecastData.timeSeries; }
    
    // Data extraction for graphs
    std::vector<float> extractValuesForGraph() const;
    std::vector<float> extractForecastTemperatures() const;
    std::vector<std::pair<float, int>> extractForecastTemperaturesAndSymbols() const;
    
    // Data management
    void clearCSVData();
    void clearForecastData();
    void clearAllData();
    
    // Utility methods
    size_t getCSVDataCount() const { return csvData.size(); }
    size_t getForecastDataCount() const { return forecastData.timeSeries.size(); }

private:
    // CSV parsing helpers
    bool parseCSVLine(const std::string& line, char delimiter, WeatherDataPoint& result);
    
    // JSON parsing helpers  
    bool parseForecastJSON(const std::string& jsonContent);
    std::string extractJSONString(const std::string& json, const std::string& key);
    double extractJSONDouble(const std::string& json, const std::string& key);
    int extractJSONInt(const std::string& json, const std::string& key);
};

#endif