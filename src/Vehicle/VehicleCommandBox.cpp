#include "Vehicle/VehicleCommandBox.hpp"

#include "Rendering/Grid.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Vehicle/VehicleEntityPreview.hpp"
#include "Vehicle/VehicleData.hpp"
#include "Vehicle/VehicleSpawnBehaviour.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"

#include <cctype>
#include <memory>
#include <string>
#include <vector>

namespace Stand
{
    namespace
    {
        class VehicleSearchResultGrid : public Rendering::Grid
        {
            std::vector<joaat_t> m_Hashes;
        public:
            VehicleSearchResultGrid() : Rendering::Grid(Rendering::Theme::GetContentOrigin(), 0) {}

            void SetResults(std::vector<joaat_t> hashes)
            {
                m_Hashes = std::move(hashes);
                invalidate();
            }
        protected:
            void populate(std::vector<std::unique_ptr<Rendering::GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Rendering::Theme::kContentItemHeight);
                for (auto hash : m_Hashes)
                    items_draft.push_back(std::make_unique<Rendering::VehicleEntityPreview>(
                        Rendering::Theme::kContentWidth, h, hash));
            }
        };

        VehicleSearchResultGrid g_SearchResults{};

        std::string VehicleSuggestions(const std::string& term)
        {
            if (term.size() < 2)
                return "";

            std::string lower = term;
            for (auto& c : lower)
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

            std::string out;
            int count = 0;
            for (size_t i = 0; i < g_VehicleCount && count < 5; ++i)
            {
                std::string name(g_VehicleData[i].hash_name);
                std::string nameLower = name;
                for (auto& c : nameLower)
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                if (nameLower.find(lower) != std::string::npos)
                {
                    if (!out.empty())
                        out += "  |  ";
                    out += name;
                    ++count;
                }
            }
            return out;
        }
    }

    void OpenInputModelName()
    {
        Rendering::MenuCommandBox::Open(
            "spawn", "Input Model Name", "", "",
            [](const std::string& term) -> bool {
                if (term.empty())
                    return false;
                const joaat_t hash = Stand::Joaat(term.c_str());
                if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash))
                    return false;
                FiberPool::queueJob([hash, term] {
                    Stand::SpawnVehicle(hash, term);
                });
                return true;
            },
            VehicleSuggestions
        );
    }

    void OpenVehicleSearch()
    {
        Rendering::MenuCommandBox::Open(
            "findvehicle", "Search for a vehicle to spawn", "", "",
            [](const std::string& term) -> bool {
                if (term.empty())
                    return false;

                std::string lower = term;
                for (auto& c : lower)
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

                std::vector<joaat_t> results;
                for (size_t i = 0; i < g_VehicleCount && results.size() < 30; ++i)
                {
                    std::string name(g_VehicleData[i].hash_name);
                    for (auto& c : name)
                        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                    if (name.find(lower) != std::string::npos)
                        results.push_back(Stand::Joaat(g_VehicleData[i].hash_name));
                }

                if (results.empty())
                    return false;

                g_SearchResults.SetResults(std::move(results));
                Rendering::MenuNavigation::Push("Search: " + term, &g_SearchResults);
                return true;
            },
            VehicleSuggestions
        );
    }
}
