#include "SMHITestData.h"

String SMHITestData::getForecastTestJSON() {
    return R"({
  "timeSeries": [
    {
      "time": "2025-11-29T12:00:00Z",
      "data": {
        "air_temperature": 7.7,
        "wind_speed": 4.6,
        "relative_humidity": 91,
        "air_pressure_at_mean_sea_level": 1009.2,
        "precipitation_amount_mean": 0.0
      }
    },
    {
      "time": "2025-11-30T12:00:00Z",
      "data": {
        "air_temperature": 7.4,
        "wind_speed": 5.6,
        "relative_humidity": 90,
        "air_pressure_at_mean_sea_level": 1008.7,
        "precipitation_amount_mean": 0.1
      }
    },
    {
      "time": "2025-12-01T12:00:00Z", 
      "data": {
        "air_temperature": 7.0,
        "wind_speed": 6.1,
        "relative_humidity": 89,
        "air_pressure_at_mean_sea_level": 1009.5,
        "precipitation_amount_mean": 0.0
      }
    },
    {
      "time": "2025-12-02T12:00:00Z",
      "data": {
        "air_temperature": 6.3,
        "wind_speed": 5.8,
        "relative_humidity": 87,
        "air_pressure_at_mean_sea_level": 1010.2,
        "precipitation_amount_mean": 0.2
      }
    },
    {
      "time": "2025-12-03T12:00:00Z",
      "data": {
        "air_temperature": 5.7,
        "wind_speed": 4.3,
        "relative_humidity": 85,
        "air_pressure_at_mean_sea_level": 1011.8,
        "precipitation_amount_mean": 0.0
      }
    },
    {
      "time": "2025-12-04T12:00:00Z",
      "data": {
        "air_temperature": 6.0,
        "wind_speed": 3.9,
        "relative_humidity": 82,
        "air_pressure_at_mean_sea_level": 1012.4,
        "precipitation_amount_mean": 0.1
      }
    },
    {
      "time": "2025-12-05T12:00:00Z",
      "data": {
        "air_temperature": 5.8,
        "wind_speed": 4.1,
        "relative_humidity": 84,
        "air_pressure_at_mean_sea_level": 1011.9,
        "precipitation_amount_mean": 0.0
      }
    }
  ]
})";
}

String SMHITestData::getHistoricalTemperatureTestJSON() {
    return R"({
  "parameter": {
    "key": "1",
    "name": "Lufttemperatur",
    "summary": "momentanvärde, 1 gång/tim",
    "unit": "celsius"
  },
  "value": [
    {"date": 1753146000000, "value": "19.8", "quality": "G"},
    {"date": 1753149600000, "value": "19.8", "quality": "G"},
    {"date": 1753153200000, "value": "20.1", "quality": "G"},
    {"date": 1753156800000, "value": "20.1", "quality": "G"},
    {"date": 1753160400000, "value": "20.6", "quality": "G"},
    {"date": 1753164000000, "value": "20.8", "quality": "G"},
    {"date": 1753167600000, "value": "22.1", "quality": "G"},
    {"date": 1753171200000, "value": "23.3", "quality": "G"},
    {"date": 1753174800000, "value": "24.0", "quality": "G"},
    {"date": 1753178400000, "value": "25.1", "quality": "G"},
    {"date": 1753182000000, "value": "25.5", "quality": "G"},
    {"date": 1753185600000, "value": "25.1", "quality": "G"},
    {"date": 1753189200000, "value": "24.1", "quality": "G"},
    {"date": 1753192800000, "value": "22.8", "quality": "G"},
    {"date": 1753196400000, "value": "20.9", "quality": "G"},
    {"date": 1753200000000, "value": "20.0", "quality": "G"},
    {"date": 1753203600000, "value": "19.6", "quality": "G"}
  ]
})";
}

String SMHITestData::getHistoricalHumidityTestJSON() {
    return R"({
  "parameter": {
    "name": "Relativ Luftfuktighet", 
    "unit": "procent"
  },
  "value": [
    {"date": 1753146000000, "value": "65", "quality": "Y"},
    {"date": 1753149600000, "value": "68", "quality": "Y"},
    {"date": 1753153200000, "value": "72", "quality": "Y"},
    {"date": 1753156800000, "value": "75", "quality": "Y"},
    {"date": 1753160400000, "value": "78", "quality": "Y"},
    {"date": 1753164000000, "value": "82", "quality": "Y"},
    {"date": 1753167600000, "value": "85", "quality": "Y"}
  ]
})";
}

String SMHITestData::getHistoricalPressureTestJSON() {
    return R"({
  "parameter": {
    "name": "Lufttryck",
    "unit": "hPa"
  },
  "value": [
    {"date": 1753146000000, "value": "1013.2", "quality": "G"},
    {"date": 1753149600000, "value": "1012.8", "quality": "G"},
    {"date": 1753153200000, "value": "1012.1", "quality": "G"},
    {"date": 1753156800000, "value": "1011.5", "quality": "G"}
  ]
})";
}

String SMHITestData::getHistoricalWindTestJSON() {
    return R"({
  "parameter": {
    "name": "Vindhastighet",
    "unit": "m/s"
  },
  "value": [
    {"date": 1753146000000, "value": "3.2", "quality": "G"},
    {"date": 1753149600000, "value": "4.1", "quality": "G"},
    {"date": 1753153200000, "value": "5.3", "quality": "G"},
    {"date": 1753156800000, "value": "4.8", "quality": "G"}
  ]
})";
}

std::vector<float> SMHITestData::getExpectedTemperatures() {
    return {7.7, 7.4, 7.0, 6.3, 5.7, 6.0, 5.8};
}

std::vector<float> SMHITestData::getExpectedHumidity() {
    return {91.0, 90.0, 89.0, 87.0, 85.0, 82.0, 84.0};
}

std::vector<float> SMHITestData::getExpectedWindSpeeds() {
    return {4.6, 5.6, 6.1, 5.8, 4.3, 3.9, 4.1};
}

void SMHITestData::debugTimestamps() {
    Serial.println("\n=== DEBUG TIMESTAMPS ===");
    
    // Test with one of your timestamps
    unsigned long testTimestamp = 1753146000000;
    Serial.printf("Test timestamp: %lu\n", testTimestamp);
    
    // Convert to seconds
    time_t rawTime = testTimestamp / 1000;
    Serial.printf("As seconds: %lu\n", rawTime);
    
    // Get current time for comparison
    time_t now = time(nullptr);
    Serial.printf("Current time: %lu\n", now);
    
    // Convert your timestamp
    struct tm* timeInfo = gmtime(&rawTime);
    Serial.printf("Your date: %04d-%02d-%02d %02d:%02d\n", 
                 timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, 
                 timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
    
    // Convert current time
    struct tm* nowInfo = gmtime(&now);
    Serial.printf("Current date: %04d-%02d-%02d %02d:%02d\n",
                 nowInfo->tm_year + 1900, nowInfo->tm_mon + 1,
                 nowInfo->tm_mday, nowInfo->tm_hour, nowInfo->tm_min);
}

void SMHITestData::debugJSONStructure() {
    Serial.println("\n=== DEBUG JSON STRUCTURE ===");
    
    String testJSON = getHistoricalTemperatureTestJSON();
    Serial.println("JSON length: " + String(testJSON.length()));
    Serial.println("First 200 chars:");
    Serial.println(testJSON.substring(0, 200));
    
    // Check for balanced braces
    int openBraces = 0;
    int closeBraces = 0;
    int openBrackets = 0;
    int closeBrackets = 0;
    
    for (char c : testJSON) {
        if (c == '{') openBraces++;
        if (c == '}') closeBraces++;
        if (c == '[') openBrackets++;
        if (c == ']') closeBrackets++;
    }
    
    Serial.printf("Braces: {=%d, }=%d (should be equal)\n", openBraces, closeBraces);
    Serial.printf("Brackets: [=%d, ]=%d (should be equal)\n", openBrackets, closeBrackets);
}