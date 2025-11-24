#pragma once
#include <string>
#include <ostream>

struct SMHIParameter
{
	std::string title;
	std::string description;
	int smhiParameterkey;
	int enumParameterkey;

	SMHIParameter(std::string title = "", std::string description = "", int smhiParameterkey = -1, int enumParameterkey = -1)
	{
		this->title = title;
		this->description = description;
		this->smhiParameterkey = smhiParameterkey;
		this->enumParameterkey = enumParameterkey;
	}
};