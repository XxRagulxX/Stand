#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace Stand
{
    class SpawnedVehicleMgr
    {
    public:
        struct Entry
        {
            int handle;
            std::string name;
        };

        static inline std::vector<Entry> s_Vehicles{};
        static inline std::size_t s_Version = 0;

        static void Add(int handle, std::string name);
        static void Remove(int handle);
        static void Prune();
    };
}
