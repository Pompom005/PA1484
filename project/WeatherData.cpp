#include "WeatherData.h"

WeatherData::WeatherData()
    : location("Unknown"), temperature(0.0), weather("N/A"), humidity(0.0), airSpeed(0.0) {}

void WeatherData::setLocation(const std::string& loc) { location = loc; }
void WeatherData::setTemperature(double temp) { temperature = temp; }
void WeatherData::setWeather(const std::string& condition) { weather = condition; }
void WeatherData::setHumidity(double hum) { humidity = hum; }
void WeatherData::setAirSpeed(double speed) { airSpeed = speed; }

std::string WeatherData::getLocation() const { return location; }
double WeatherData::getTemperature() const { return temperature; }
std::string WeatherData::getWeather() const { return weather; }
double WeatherData::getHumidity() const { return humidity; }
double WeatherData::getAirSpeed() const { return airSpeed; }
