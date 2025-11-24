#pragma once
#include <vector>
#include <map>
#include "SMHIStation.h"
#include "SMHIParameter.h"

struct DropdownParameter
{
    SMHIParameter* realParameter;
};

struct DropdownStation
{
    SMHIStation* realStation;
};

class SMHIStationsAndParameters
{
private:
    std::vector<SMHIStation*> stations;
    std::vector<SMHIParameter*> parameters;
    std::map<int, DropdownParameter> parametersMap;
    std::map<int, std::vector<DropdownStation>> eligibleStations;
public:
    //Store all stations in a map, same int for the parameter type but point at 
    //vector with stations that are available for it
    const SMHIParameter& GetParameter(int parameter);
    const SMHIParameter& GetParameter(SupportedParameter parameter);
    const std::vector<DropdownParameter> GetParameters();
    const std::vector<DropdownStation>& GetEligibleStations(int parameter);
    const std::vector<DropdownStation>& GetEligibleStations(SupportedParameter parameter);

    static SMHIStationsAndParameters& GetInstance()
    {
        static SMHIStationsAndParameters instance;
        return instance;
    }

    void Init();
};

std::ostream& operator << (std::ostream &os, const DropdownParameter &s);
std::ostream& operator << (std::ostream &os, const DropdownStation &s);