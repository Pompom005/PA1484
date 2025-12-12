#pragma once
#include <vector>
#include <map>
#include "SMHIStation.h"
#include "SMHIParameter.h"

//These dropdown versions are only used to we can have '<<' operators without having to use references or stack objs. (Because its dynamic and not small enough for the stack)
struct DropdownParameter
{
    SMHIParameter* real_parameter;
};

struct DropdownStation
{
    SMHIStation* real_station;
};

class SMHIStationsAndParameters
{
private:
    std::vector<SMHIStation*> stations; //All station objects
    std::vector<SMHIParameter*> parameters; //All parameter objects
    std::map<int, DropdownParameter> parameters_map; //Parameters mapped by enum int. (2^x)
    std::map<int, std::vector<DropdownStation>> eligible_stations; //All stations that have the relevant parameter bit set
public:
    //Store all stations in a map, same int for the parameter type but point at 
    //vector with stations that are available for it
    const SMHIParameter& get_parameter(int parameter);
    const SMHIParameter& get_parameter(SupportedParameter parameter);
    const std::vector<DropdownParameter> get_parameters();
    const std::vector<DropdownStation>& get_eligible_stations(int parameter);
    const std::vector<DropdownStation>& get_eligible_stations(SupportedParameter parameter);

    static SMHIStationsAndParameters& instance() //Singleton because we want this like everywhere
    {
        static SMHIStationsAndParameters instance;
        return instance;
    }

    void Init(); //Loads all stations and parameters and sets up maps.
};

//Operators for the dropdown parameter and stations
std::ostream& operator << (std::ostream &os, const DropdownParameter &s);
std::ostream& operator << (std::ostream &os, const DropdownStation &s);