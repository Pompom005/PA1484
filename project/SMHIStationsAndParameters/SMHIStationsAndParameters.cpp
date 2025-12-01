#include "SMHIStationsAndParameters.h"
#include "StationAndParameterData.h"
#include <iostream>
#include <HWCDC.h>

const SMHIParameter &SMHIStationsAndParameters::GetParameter(int parameter)
{
    return *parameters[parameter];
}

const SMHIParameter &SMHIStationsAndParameters::GetParameter(SupportedParameter parameter)
{
    return GetParameter(static_cast<int>(parameter));
}

const std::vector<DropdownParameter> SMHIStationsAndParameters::GetParameters()
{   
    std::vector<DropdownParameter> toReturn;
    for(int i = 0; i < parameters.size(); i++)
    {
        DropdownParameter param;
        param.realParameter = parameters[i];
        toReturn.push_back(param);
    }
    return toReturn;
}

const std::vector<DropdownStation> &SMHIStationsAndParameters::GetEligibleStations(int parameter)
{
    return eligibleStations[parameter];
}

const std::vector<DropdownStation> &SMHIStationsAndParameters::GetEligibleStations(SupportedParameter parameter)
{
    return GetEligibleStations(static_cast<int>(parameter));
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
        parameter.realParameter  = parameters[i];
        parametersMap[parameter.realParameter->enumParameterkey] = parameter;

        eligibleStations[parameter.realParameter->enumParameterkey] = std::vector<DropdownStation>();
        std::vector<DropdownStation>& eligibleStationsVec = eligibleStations[parameter.realParameter->enumParameterkey];
        for(int j = 0; j < stations.size(); j++)
        {
            DropdownStation station;
            station.realStation = stations[j];
            if(station.realStation->supportedParameters & parameter.realParameter->enumParameterkey) //This parameter is supported on this station
            {
                eligibleStationsVec.push_back(station);
            }
        }
    }
    Serial.println("Created maps");
}

std::ostream &operator<<(std::ostream &os, const DropdownParameter &s)
{
    std::string str = s.realParameter->title;
    return (os << str.substr(0, str.find(":", 0)));
}
std::ostream &operator<<(std::ostream &os, const DropdownStation &s)
{
    return (os << s.realStation->name);
}
