#include "Vehicle/GarageVehicleMgr.hpp"

#include "Core/FileMgr.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/Vehicle.hpp"
#include "Vehicle/VehicleData.hpp"
#include "World/Self.hpp"

#include <nlohmann/json.hpp>
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
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
            {
                if (entry.path().extension() == ".txt")
                    s_Names.push_back(entry.path().stem().generic_string());
            }
            std::sort(s_Names.begin(), s_Names.end());
        }
        catch (...) {}

        ++s_Version;
    }

    void GarageVehicleMgr::Save(const std::string& name, int vehicleHandle)
    {
        const int veh = vehicleHandle;

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

        nlohmann::json j;
        j["Model"]              = GetModelName(modelHash);
        j["Plate Style"]        = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh);
        j["Plate Text"]         = std::string(VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh));
        j["Primary Colour"]     = primaryColour;
        j["Secondary Colour"]   = secondaryColour;
        j["Pearl Colour"]       = pearlColour;
        j["Wheel Colour"]       = wheelColour;
        j["Wheel Type"]         = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);

        j["Spoiler"]      = VEHICLE::GET_VEHICLE_MOD(veh, 0);
        j["Front Bumper"] = VEHICLE::GET_VEHICLE_MOD(veh, 1);
        j["Rear Bumper"]  = VEHICLE::GET_VEHICLE_MOD(veh, 2);
        j["Sideskirt"]    = VEHICLE::GET_VEHICLE_MOD(veh, 3);
        j["Exhaust"]      = VEHICLE::GET_VEHICLE_MOD(veh, 4);
        j["Grille"]       = VEHICLE::GET_VEHICLE_MOD(veh, 6);
        j["Hood"]         = VEHICLE::GET_VEHICLE_MOD(veh, 7);
        j["Fender"]       = VEHICLE::GET_VEHICLE_MOD(veh, 8);
        j["Roof"]         = VEHICLE::GET_VEHICLE_MOD(veh, 10);
        j["Engine"]       = VEHICLE::GET_VEHICLE_MOD(veh, 11);
        j["Brakes"]       = VEHICLE::GET_VEHICLE_MOD(veh, 12);
        j["Transmission"] = VEHICLE::GET_VEHICLE_MOD(veh, 13);
        j["Horns"]        = VEHICLE::GET_VEHICLE_MOD(veh, 14);
        j["Suspension"]   = VEHICLE::GET_VEHICLE_MOD(veh, 15);
        j["Armor"]        = VEHICLE::GET_VEHICLE_MOD(veh, 16);

        j["Nitrous"]   = static_cast<bool>(VEHICLE::IS_TOGGLE_MOD_ON(veh, 17));
        j["Turbo"]     = static_cast<bool>(VEHICLE::IS_TOGGLE_MOD_ON(veh, 18));
        j["Subwoofer"] = static_cast<bool>(VEHICLE::IS_TOGGLE_MOD_ON(veh, 19));
        j["Tiresmoke"] = static_cast<bool>(VEHICLE::IS_TOGGLE_MOD_ON(veh, 20));
        j["Unk21"]     = static_cast<bool>(VEHICLE::IS_TOGGLE_MOD_ON(veh, 21));

        j["Headlights"]   = VEHICLE::GET_VEHICLE_MOD(veh, 22);
        j["Front Wheels"] = VEHICLE::GET_VEHICLE_MOD(veh, 23);
        j["Livery"]       = VEHICLE::GET_VEHICLE_LIVERY(veh);

        j["Tyre Smoke Colour"] = RgbToHex(smokR, smokG, smokB);
        j["Window Tint"]       = VEHICLE::GET_VEHICLE_WINDOW_TINT(veh);

        if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
        {
            int r = 0, g = 0, b = 0;
            VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &r, &g, &b);
            j["Custom Primary Colour"] = RgbToHex(r, g, b);
        }
        else
        {
            j["Custom Primary Colour"] = "";
        }

        if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh))
        {
            int r = 0, g = 0, b = 0;
            VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &r, &g, &b);
            j["Custom Secondary Colour"] = RgbToHex(r, g, b);
        }
        else
        {
            j["Custom Secondary Colour"] = "";
        }

        j["Neon Colour"] = RgbToHex(neonR, neonG, neonB);
        j["Neon Front"]  = static_cast<bool>(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 0));
        j["Neon Back"]   = static_cast<bool>(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 1));
        j["Neon Left"]   = static_cast<bool>(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 2));
        j["Neon Right"]  = static_cast<bool>(VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 3));

        j["Interior Colour"]  = interiorColour;
        j["Dashboard Colour"] = dashColour;
        j["Tyres Mode"]       = static_cast<int>(VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh));

        nlohmann::json extras = nlohmann::json::object();
        for (int id = 1; id <= 12; ++id)
            if (VEHICLE::DOES_EXTRA_EXIST(veh, id))
                extras[std::to_string(id)] = static_cast<bool>(VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, id));
        j["Extras"] = extras;

        try
        {
            const auto folder = GetVehiclesFolder();
            std::filesystem::create_directories(folder);
            std::ofstream file(GetVehiclePath(name), std::ofstream::trunc | std::ofstream::binary);
            if (file.is_open())
                file << j.dump(4);
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

        nlohmann::json j;
        try
        {
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open()) return;
            file >> j;
        }
        catch (...) { return; }

        const std::string modelName = j.value("Model", "");
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

        VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(h, j.value("Plate Text", "").c_str());
        VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(h, j.value("Plate Style", 0));

        const int primary   = j.value("Primary Colour",   0);
        const int secondary = j.value("Secondary Colour", 0);
        VEHICLE::SET_VEHICLE_COLOURS(h, primary, secondary);
        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(h, j.value("Pearl Colour", 0), j.value("Wheel Colour", 0));
        VEHICLE::SET_VEHICLE_WHEEL_TYPE(h, j.value("Wheel Type", 0));
        VEHICLE::SET_VEHICLE_WINDOW_TINT(h, j.value("Window Tint", 0));

        const std::string cpc = j.value("Custom Primary Colour", "");
        if (!cpc.empty())
        {
            int r = 0, g = 0, b = 0;
            if (HexToRgb(cpc, r, g, b))
                VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(h, r, g, b);
        }
        const std::string csc = j.value("Custom Secondary Colour", "");
        if (!csc.empty())
        {
            int r = 0, g = 0, b = 0;
            if (HexToRgb(csc, r, g, b))
                VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(h, r, g, b);
        }

        VEHICLE::SET_VEHICLE_MOD(h, 0,  j.value("Spoiler",      -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 1,  j.value("Front Bumper", -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 2,  j.value("Rear Bumper",  -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 3,  j.value("Sideskirt",    -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 4,  j.value("Exhaust",      -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 6,  j.value("Grille",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 7,  j.value("Hood",         -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 8,  j.value("Fender",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 10, j.value("Roof",         -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 11, j.value("Engine",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 12, j.value("Brakes",       -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 13, j.value("Transmission", -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 14, j.value("Horns",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 15, j.value("Suspension",   -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 16, j.value("Armor",        -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 22, j.value("Headlights",   -1), FALSE);
        VEHICLE::SET_VEHICLE_MOD(h, 23, j.value("Front Wheels", -1), FALSE);

        VEHICLE::TOGGLE_VEHICLE_MOD(h, 17, j.value("Nitrous",   false) ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 18, j.value("Turbo",     false) ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 19, j.value("Subwoofer", false) ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 20, j.value("Tiresmoke", false) ? TRUE : FALSE);
        VEHICLE::TOGGLE_VEHICLE_MOD(h, 21, j.value("Unk21",     false) ? TRUE : FALSE);

        VEHICLE::SET_VEHICLE_LIVERY(h, j.value("Livery", -1));

        {
            int r = 255, g = 255, b = 255;
            HexToRgb(j.value("Tyre Smoke Colour", "#ffffff"), r, g, b);
            VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(h, r, g, b);
        }
        {
            int r = 0, g = 0, b = 0;
            HexToRgb(j.value("Neon Colour", "#000000"), r, g, b);
            VEHICLE::SET_VEHICLE_NEON_COLOUR(h, r, g, b);
        }
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 0, j.value("Neon Front", false) ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 1, j.value("Neon Back",  false) ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 2, j.value("Neon Left",  false) ? TRUE : FALSE);
        VEHICLE::SET_VEHICLE_NEON_ENABLED(h, 3, j.value("Neon Right", false) ? TRUE : FALSE);

        VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(h, j.value("Interior Colour",  0));
        VEHICLE::SET_VEHICLE_EXTRA_COLOUR_6(h, j.value("Dashboard Colour", 0));
        VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(h, j.value("Tyres Mode", 1) != 0 ? TRUE : FALSE);

        if (j.contains("Extras") && j["Extras"].is_object())
        {
            for (auto& [key, val] : j["Extras"].items())
            {
                try
                {
                    const int id = std::stoi(key);
                    const bool on = val.get<bool>();
                    if (VEHICLE::DOES_EXTRA_EXIST(h, id))
                        VEHICLE::SET_VEHICLE_EXTRA(h, id, on ? FALSE : TRUE);
                }
                catch (...) {}
            }
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
            std::string nameLower = n;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(lower) != std::string::npos)
                result.push_back(n);
        }
        return result;
    }
}
