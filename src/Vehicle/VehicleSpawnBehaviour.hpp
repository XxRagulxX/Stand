#pragma once
#include "Util/Joaat.hpp"

#include <string>

namespace Stand
{
    void SpawnVehicleOnFoot(joaat_t hash, const std::string& name);
    void SpawnVehicleInVehicle(joaat_t hash, const std::string& name);
    void SpawnVehicle(joaat_t hash, const std::string& name);
    void SpawnVehicleAndDrive(joaat_t hash, const std::string& name);
}
