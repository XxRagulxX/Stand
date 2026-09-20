#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace Stand
{
    class GarageVehicleMgr
    {
    public:
        static inline std::vector<std::string> s_Names{};
        static inline std::size_t s_Version = 0;
        static inline bool s_Loaded = false;

        static void LoadList();
        static void Save(const std::string& name, int vehicleHandle);
        static void Spawn(const std::string& name);
        static void Delete(const std::string& name);
        static std::vector<std::string> Filter(const std::string& query);
    };
}
