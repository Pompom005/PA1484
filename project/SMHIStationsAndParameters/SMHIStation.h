#pragma once
#include <string>
#include <ostream>
enum class SupportedParameter
{
	WindDirection = 1 << 0,
	WindSpeed = 1 << 1,
	TotalCloudAmount = 1 << 2,
	SunshineTime = 1 << 3,
	SnowDepth = 1 << 4,
	View = 1 << 5,
	RelativeAirHumidity = 1 << 6,
	PrecipitationAmountHourly = 1 << 7,
	PrecipitationAmountDaily = 1 << 8,
	PrecipitationAmountMonthly = 1 << 9,
	PrecipitationDaily = 1 << 10,
	PrecipitationTwiceDaily = 1 << 11,
	CloudAmountFourthLayer = 1 << 12,
	CloudAmountThirdLayer = 1 << 13,
	CloudAmountSecondLayer = 1 << 14,
	CloudAmountFirstLayer = 1 << 15,
	WindSpeedMaxAverage = 1 << 16,
	AirPressure = 1 << 17,
	AirTemperatureMaxDaily = 1 << 18,
	AirTemperatureAverageDaily = 1 << 19,
	AirTemperatureMoment = 1 << 20,
	AirTemperatureMinDaily = 1 << 21,
	AirTemperatureMaxTwiceDaily = 1 << 22,
	AirTemperatureMinTwiceDaily = 1 << 23,
	AirTemperatureAverageMonthly = 1 << 24,
	DewPointTemperature = 1 << 25
};

struct SMHIStation
{
	std::string name;
	int key; //Nr used in url for smhi calls
	float latitude; 
	float longitude;
	int supported_parameters = 0; //Represents all supported parameters, each bit corresponds to a parameter

	SMHIStation(std::string name = "", int key = -1, float latitude = 0, float longitude = 0, int supportedParameters = 0)
	{
		this->name = name;
		this->key = key;
		this->latitude = latitude;
		this->longitude = longitude;
		this->supported_parameters = supportedParameters;
	}
};
