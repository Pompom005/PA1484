#include "SMHIStationsAndParameters.h"
#include "StationAndParameterData.h"
#include <iostream>
#include <HWCDC.h>

const SMHIParameter &SMHIStationsAndParameters::get_parameter(int parameter)
{
   return *parameters_map[parameter].real_parameter;
}

const SMHIParameter &SMHIStationsAndParameters::get_parameter(SupportedParameter parameter)
{
    return get_parameter(static_cast<int>(parameter));
}

const std::vector<DropdownParameter> SMHIStationsAndParameters::get_parameters()
{   
    std::vector<DropdownParameter> toReturn;
    for(int i = 0; i < parameters.size(); i++)
    {
        DropdownParameter param;
        param.real_parameter = parameters[i];
        toReturn.push_back(param);
    }
    return toReturn;
}

const std::vector<DropdownStation> &SMHIStationsAndParameters::get_eligible_stations(int parameter)
{
    return eligible_stations[parameter];
}

const std::vector<DropdownStation> &SMHIStationsAndParameters::get_eligible_stations(SupportedParameter parameter)
{
    return get_eligible_stations(static_cast<int>(parameter));
}

void SMHIStationsAndParameters::Init()
{
    Serial.println("Started Init");
    FillWithParameters(parameters);
    Serial.println("Filled params");
    FillWithStations(stations);
    Serial.println("Filled stations");

    //Create the maps for easier and faster navigation
    for(int i = 0; i < parameters.size(); i++)
    {
        DropdownParameter parameter;
        parameter.real_parameter  = parameters[i];
        parameters_map[parameter.real_parameter->enum_parameter_key] = parameter;

        eligible_stations[parameter.real_parameter->enum_parameter_key] = std::vector<DropdownStation>();
        std::vector<DropdownStation>& eligibleStationsVec = eligible_stations[parameter.real_parameter->enum_parameter_key];
        for(int j = 0; j < stations.size(); j++)
        {
            DropdownStation station;
            station.real_station = stations[j];
            if(station.real_station->supported_parameters & parameter.real_parameter->enum_parameter_key) //This parameter is supported on this station
            {
                eligibleStationsVec.push_back(station);
            }
        }
    }
    Serial.println("Created maps");
}

std::ostream &operator<<(std::ostream &os, const DropdownParameter &s)
{
    std::string str = s.real_parameter->title;
    return (os << str.substr(0, str.find(":", 0)));
}
std::ostream &operator<<(std::ostream &os, const DropdownStation &s)
{
    return (os << s.real_station->name);
}
