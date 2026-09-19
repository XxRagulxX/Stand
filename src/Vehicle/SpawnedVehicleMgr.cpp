#include "Vehicle/SpawnedVehicleMgr.hpp"

#include "Vehicle/Vehicle.hpp"

#include <algorithm>
#include <string>

namespace Stand
{
    void SpawnedVehicleMgr::Add(int handle, std::string name)
    {
        std::size_t duplicates = 0;
        for (const auto& e : s_Vehicles)
        {
            if (e.name.find(name) != std::string::npos)
                ++duplicates;
        }
        if (duplicates > 0)
            name += " (#" + std::to_string(duplicates + 1) + ")";

        s_Vehicles.push_back({handle, std::move(name)});
        ++s_Version;
    }

    void SpawnedVehicleMgr::Remove(int handle)
    {
        auto it = std::find_if(s_Vehicles.begin(), s_Vehicles.end(),
            [handle](const Entry& e) { return e.handle == handle; });
        if (it != s_Vehicles.end())
        {
            s_Vehicles.erase(it);
            ++s_Version;
        }
    }

    void SpawnedVehicleMgr::Prune()
    {
        bool changed = false;
        for (auto it = s_Vehicles.begin(); it != s_Vehicles.end(); )
        {
            if (!Vehicle(it->handle).IsValid())
            {
                it = s_Vehicles.erase(it);
                changed = true;
            }
            else
                ++it;
        }
        if (changed)
            ++s_Version;
    }
}
