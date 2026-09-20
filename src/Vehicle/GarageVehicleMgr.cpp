#include "Vehicle/GarageVehicleMgr.hpp"

#include "Core/FileMgr.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/Vehicle.hpp"
#include "Vehicle/VehicleData.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <string>

namespace Stand
{
    namespace
    {
        std::filesystem::path GetVehiclesFolder()
        {
            return FileMgr::GetProjectFolder("./Vehicles").Path();
        }

        std::filesystem::path GetVehiclePath(const std::string& name)
        {
            return GetVehiclesFolder() / (name + ".txt");
        }

        std::string RgbToHex(int r, int g, int b)
        {
            return std::format("#{:02x}{:02x}{:02x}", r, g, b);
        }

        bool HexToRgb(const std::string& hex, int& r, int& g, int& b)
        {
            if (hex.size() != 7 || hex[0] != '#')
                return false;
            try
            {
                r = std::stoi(hex.substr(1, 2), nullptr, 16);
                g = std::stoi(hex.substr(3, 2), nullptr, 16);
                b = std::stoi(hex.substr(5, 2), nullptr, 16);
                return true;
            }
            catch (...) { return false; }
        }

        std::string GetModelName(joaat_t hash)
        {
            for (std::size_t i = 0; i < g_VehicleCount; ++i)
                if (Joaat(g_VehicleData[i].hash_name) == hash)
                    return g_VehicleData[i].hash_name;
            return std::to_string(hash);
        }

        using DataMap = std::map<std::string, std::string>;

        DataMap ParseFile(std::ifstream& file)
        {
            DataMap data;
            std::string line;
            while (std::getline(file, line))
            {
                auto pos = line.find(": ");
                if (pos == std::string::npos)
                    continue;
                data[line.substr(0, pos)] = line.substr(pos + 2);
            }
            return data;
        }

        int   GetInt(const DataMap& d, const std::string& k, int def = 0)
        {
            auto it = d.find(k);
            if (it == d.end()) return def;
            try { return std::stoi(it->second); }
            catch (...) { return def; }
        }

        bool  GetBool(const DataMap& d, const std::string& k, bool def = false)
        {
            auto it = d.find(k);
            if (it == d.end()) return def;
            return it->second == "Yes";
        }

        std::string GetStr(const DataMap& d, const std::string& k, const std::string& def = "")
        {
            auto it = d.find(k);
            if (it == d.end()) return def;
            return it->second;
        }

        const char* YN(bool v) { return v ? "Yes" : "No"; }
    }

    void GarageVehicleMgr::LoadList()
    {
        s_Loaded = true;
        s_Names.clear();

        const auto folder = GetVehiclesFolder();
        std::filesystem::create_directories(folder);

        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(folder))
                if (entry.path().extension() == ".txt")
                    s_Names.push_back(entry.path().stem().generic_string());
            std::sort(s_Names.begin(), s_Names.end());
        }
        catch (...) {}

        ++s_Version;
    }

    void GarageVehicleMgr::Save(const std::string& name, int veh)
    {
        const joaat_t modelHash = static_cast<joaat_t>(ENTITY::GET_ENTITY_MODEL(veh));

        int primaryColour = 0, secondaryColour = 0;
        VEHICLE::GET_VEHICLE_COLOURS(veh, &primaryColour, &secondaryColour);

        int pearlColour = 0, wheelColour = 0;
        VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearlColour, &wheelColour);

        int smokR = 255, smokG = 255, smokB = 255;
        VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &smokR, &smokG, &smokB);

        int neonR = 0, neonG = 0, neonB = 0;
        VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &neonR, &neonG, &neonB);

        int interiorColour = 0, dashColour = 0;
        VEHICLE::GET_VEHICLE_EXTRA_COLOUR_5(veh, &interiorColour);
        VEHICLE::GET_VEHICLE_EXTRA_COLOUR_6(veh, &dashColour);

        std::string customPrimary, customSecondary;
        if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
        {
            int r = 0, g = 0, b = 0;
            VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &r, &g, &b);
            customPrimary = RgbToHex(r, g, b);
        }
        if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh))
        {
            int r = 0, g = 0, b = 0;
            VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &r, &g, &b);
            customSecondary = RgbToHex(r, g, b);
        }

        try
        {
            std::filesystem::create_directories(GetVehiclesFolder());
            std::ofstream f(GetVehiclePath(name), std::ofstream::trunc);
            if (!f.is_open()) return;

            f << "Model: "              << GetModelName(modelHash)                             << "\n";
            f << "Plate Style: "        << VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh)   << "\n";
            f << "Plate Text: "         << VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh)         << "\n";
            f << "Primary Colour: "     << primaryColour                                       << "\n";
            f << "Secondary Colour: "   << secondaryColour                                     << "\n";
            f << "Pearl Colour: "       << pearlColour                                         << "\n";
            f << "Wheel Colour: "       << wheelColour                                         << "\n";
            f << "Wheel Type: "         << VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh)                << "\n";
            f << "Spoiler: "            << VEHICLE::GET_VEHICLE_MOD(veh, 0)                    << "\n";
            f << "Front Bumper: "       << VEHICLE::GET_VEHICLE_MOD(veh, 1)                    << "\n";
            f << "Rear Bumper: "        << VEHICLE::GET_VEHICLE_MOD(veh, 2)                    << "\n";
            f << "Sideskirt: "          << VEHICLE::GET_VEHICLE_MOD(veh, 3)                    << "\n";
            f << "Exhaust: "            << VEHICLE::GET_VEHICLE_MOD(veh, 4)                    << "\n";
            f << "Grille: "             << VEHICLE::GET_VEHICLE_MOD(veh, 6)                    << "\n";
            f << "Hood: "               << VEHICLE::GET_VEHICLE_MOD(veh, 7)                    << "\n";
            f << "Fender: "             << VEHICLE::GET_VEHICLE_MOD(veh, 8)                    << "\n";
            f << "Roof: "               << VEHICLE::GET_VEHICLE_MOD(veh, 10)                   << "\n";
            f << "Engine: "             << VEHICLE::GET_VEHICLE_MOD(veh, 11)                   << "\n";
            f << "Brakes: "             << VEHICLE::GET_VEHICLE_MOD(veh, 12)                   << "\n";
            f << "Transmission: "       << VEHICLE::GET_VEHICLE_MOD(veh, 13)                   << "\n";
            f << "Horns: "              << VEHICLE::GET_VEHICLE_MOD(veh, 14)                   << "\n";
            f << "Suspension: "         << VEHICLE::GET_VEHICLE_MOD(veh, 15)                   << "\n";
            f << "Armor: "              << VEHICLE::GET_VEHICLE_MOD(veh, 16)                   << "\n";
            f << "Nitrous: "            << YN(VEHICLE::IS_TOGGLE_MOD_ON(veh, 17))             << "\n";
            f << "Turbo: "              << YN(VEHICLE::IS_TOGGLE_MOD_ON(veh, 18))             << "\n";
            f << "Subwoofer: "          << YN(VEHICLE::IS_TOGGLE_MOD_ON(veh, 19))             << "\n";
            f << "Tiresmoke: "          << YN(VEHICLE::IS_TOGGLE_MOD_ON(veh, 20))             << "\n";
            f << "Unk21: "              << YN(VEHICLE::IS_TOGGLE_MOD_ON(veh, 21))             << "\n";
            f << "Headlights: "         << VEHICLE::GET_VEHICLE_MOD(veh, 22)                   << "\n";
            f << "Front Wheels: "       << VEHICLE::GET_VEHICLE_MOD(veh, 23)                   << "\n";
            f << "Livery: "             << VEHICLE::GET_VEHICLE_LIVERY(veh)                    << "\n";
            f << "Tyre Smoke Colour: "  << RgbToHex(smokR, smokG, smokB)                       << "\n";
            f << "Window Tint: "        << VEHICLE::GET_VEHICLE_WINDOW_TINT(veh)               << "\n";
            f << "Custom Primary Colour: "   << customPrimary                                  << "\n";
            f << "Custom Secondary Colour: " << customSecondary                                << "\n";
            f << "Neon Colour: "        << RgbToHex(neonR, neonG, neonB)                       << "\n";
            f << "Neon Front: "         << YN(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 0))      << "\n";
            f << "Neon Back: "          << YN(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 1))      << "\n";
            f << "Neon Left: "          << YN(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 2))      << "\n";
            f << "Neon Right: "         << YN(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 3))      << "\n";
            f << "Interior Colour: "    << interiorColour                                      << "\n";
            f << "Dashboard Colour: "   << dashColour                                          << "\n";
            f << "Tyres Mode: "         << static_cast<int>(VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh)) << "\n";

            for (int id = 1; id <= 12; ++id)
                if (VEHICLE::DOES_EXTRA_EXIST(veh, id))
                    f << "Extra " << id << ": " << YN(VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, id)) << "\n";
        }
        catch (...) {}

        if (std::find(s_Names.begin(), s_Names.end(), name) == s_Names.end())
        {
            s_Names.push_back(name);
            std::sort(s_Names.begin(), s_Names.end());
            ++s_Version;
        }
    }

    void GarageVehicleMgr::Spawn(const std::string& name)
    {
        const auto path = GetVehiclePath(name);
        if (!std::filesystem::exists(path))
            return;

        DataMap data;
        try
        {
            std::ifstream file(path);
            if (!file.is_open()) return;
            data = ParseFile(file);
        }
        catch (...) { return; }

        const std::string modelName = GetStr(data, "Model");
        if (modelName.empty()) return;

        const joaat_t hash = Joaat(modelName.c_str());
        if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash)) return;

        const auto loc = Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), hash);
        auto veh = Vehicle::Create(hash,
            rage::fvector3{ loc.x, loc.y, loc.z },
            Self::GetPed().GetHeading());
        if (!veh.IsValid()) return;

        Script::current()->yield(200);
        const int h = veh.GetHandle();

        VEHICLE::SET_VEHICLE_MOD_KIT(h, 0);

        VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(h, GetStr(data, "Plate Text").c_str());
        VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(h, GetInt(data, "Plate Style"));

        VEHICLE::SET_VEHICLE_COLOURS(h, GetInt(data, "Primary Colour"), GetInt(data, "Secondary Colour"));
        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(h, GetInt(data, "Pearl Colour"), GetInt(data, "Wheel Colour"));
        VEHICLE::SET_VEHICLE_WHEEL_TYPE(h, GetInt(data, "Wheel Type"));
        VEHICLE::SET_VEHICLE_WINDOW_TINT(h, GetInt(data, "Window Tint"));

        const std::string cpc = GetStr(data, "Custom Primary Colour");
        if (!cpc.empty()) { int r=0,g=0,b=0; if (HexToRgb(cpc,r,g,b)) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(h,r,g,b); }
        const std::string csc = GetStr(data, "Custom Secondary Colour");
        if (!csc.empty()) { int r=0,g=0,b=0; if (HexToRgb(csc,r,g,b)) VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(h,r,g,b); }

        VEHICLE::SET_VEHICLE_MOD(h,  0, GetInt(data, "Spoiler",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  1, GetInt(data, "Front Bumper",  -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  2, GetInt(data, "Rear Bumper",   -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  3, GetInt(data, "Sideskirt",     -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  4, GetInt(data, "Exhaust",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  6, GetInt(data, "Grille",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  7, GetInt(data, "Hood",          -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h,  8, GetInt(data, "Fender",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 10, GetInt(data, "Roof",          -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 11, GetInt(data, "Engine",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 12, GetInt(data, "Brakes",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 13, GetInt(data, "Transmission",  -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 14, GetInt(data, "Horns",         -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 15, GetInt(data, "Suspension",    -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 16, GetInt(data, "Armor",         -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 22, GetInt(data, "Headlights",    -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 23, GetInt(data, "Front Wheels",  -1), FALSE);

        VEHICLE::TOGGLE_VEHICLE_MOD(h, 17, GetBool(data, "Nitrous")   ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 18, GetBool(data, "Turbo")     ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 19, GetBool(data, "Subwoofer") ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 20, GetBool(data, "Tiresmoke") ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 21, GetBool(data, "Unk21")     ? TRUE : FALSE);

        VEHICLE::SET_VEHICLE_LIVERY(h, GetInt(data, "Livery", -1));

        { int r=255,g=255,b=255; HexToRgb(GetStr(data,"Tyre Smoke Colour","#ffffff"),r,g,b); VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(h,r,g,b); }
        { int r=0,g=0,b=0;       HexToRgb(GetStr(data,"Neon Colour","#000000"),r,g,b);       VEHICLE::SET_VEHICLE_NEON_COLOUR(h,r,g,b); }

        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 0, GetBool(data, "Neon Front") ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 1, GetBool(data, "Neon Back")  ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 2, GetBool(data, "Neon Left")  ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 3, GetBool(data, "Neon Right") ? TRUE : FALSE);

        VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(h, GetInt(data, "Interior Colour"));
        VEHICLE::SET_VEHICLE_EXTRA_COLOUR_6(h, GetInt(data, "Dashboard Colour"));
        VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(h, GetInt(data, "Tyres Mode", 1) != 0 ? TRUE : FALSE);

        for (int id = 1; id <= 12; ++id)
        {
            const std::string key = "Extra " + std::to_string(id);
            auto it = data.find(key);
            if (it == data.end()) continue;
            if (VEHICLE::DOES_EXTRA_EXIST(h, id))
                VEHICLE::SET_VEHICLE_EXTRA(h, id, it->second == "Yes" ? FALSE : TRUE);
        }
    }

    void GarageVehicleMgr::Delete(const std::string& name)
    {
        try { std::filesystem::remove(GetVehiclePath(name)); }
        catch (...) {}

        auto it = std::find(s_Names.begin(), s_Names.end(), name);
        if (it != s_Names.end())
        {
            s_Names.erase(it);
            ++s_Version;
        }
    }

    std::vector<std::string> GarageVehicleMgr::Filter(const std::string& query)
    {
        if (query.empty())
            return s_Names;

        std::string lower = query;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        std::vector<std::string> result;
        for (const auto& n : s_Names)
        {
            std::string nl = n;
            std::transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
            if (nl.find(lower) != std::string::npos)
                result.push_back(n);
        }
        return result;
    }
}
