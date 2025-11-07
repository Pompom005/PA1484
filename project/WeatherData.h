#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <string>

class WeatherData {
private:
    std::string location;
    double temperature;
    std::string weather;
    double humidity;
    double airSpeed;

public:
    WeatherData();  // Constructor
    void setLocation(const std::string& loc);
    void setTemperature(double temp);
    void setWeather(const std::string& condition);
    void setHumidity(double hum);
    void setAirSpeed(double speed);

    std::string getLocation() const;
    double getTemperature() const;
    std::string getWeather() const;
    double getHumidity() const;
    double getAirSpeed() const;
};

#endif
