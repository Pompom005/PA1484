#pragma once
#include <string>
#include <ostream>

struct SMHIParameter
{
	std::string title; //Title of the data
	std::string description; //Description of what the data represents
	int smhi_parameter_key; //This is the nr used in the url
	int enum_parameter_key; //This is the nr of the enum to represent it, always a 2^x (SupportedParameter) Used in stations to see if bit is set

	SMHIParameter(std::string title = "", std::string description = "", int smhiParameterkey = -1, int enumParameterkey = -1)
	{
		this->title = title;
		this->description = description;
		this->smhi_parameter_key = smhiParameterkey;
		this->enum_parameter_key = enumParameterkey;
	}
};