#pragma once
#include "Util/Joaat.hpp"
#include <array>

struct VehicleEntry {
    const char* hash_name;
    const char* dlc_name;
};

extern const VehicleEntry g_VehicleData[];
extern const size_t       g_VehicleCount;

namespace Stand
{
    static constexpr auto g_VehicleClassNames = std::to_array({
        "Compact",
        "Sedan",
        "SUV",
        "Coupe",
        "Muscle",
        "Sport Classic",
        "Sport",
        "Super",
        "Motorcycle",
        "Off-road",
        "Industrial",
        "Utility",
        "Van",
        "Cycle",
        "Boat",
        "Helicopter",
        "Plane",
        "Service",
        "Emergency",
        "Military",
        "Commercial",
        "Rail",
        "Open Wheel",
    });
}
