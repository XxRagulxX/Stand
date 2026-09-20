#pragma once
#include <cstddef>
#include <string>

namespace Stand::Features
{
    enum class PVSort { Name, Slot };

    extern std::size_t g_PVVersion;

    std::string GetPVFilter();
    PVSort      GetPVSort();
    void        SetPVSort(PVSort sort);
    void        OpenPVSearch();
    void        SaveAllPVsToGarage();
}
