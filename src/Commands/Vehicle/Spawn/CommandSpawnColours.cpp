#include "Commands/Vehicle/Spawn/CommandSpawnColours.hpp"

#include "Commands/CommandLegacy.hpp"

namespace Stand::Features
{
    namespace
    {
        class CommandSpawnColour : public CommandLegacy
        {
            ImVec4 m_State = ImVec4(1.f, 1.f, 1.f, 1.f);
        public:
            CommandSpawnColour(const char* name, const char* label, const char* desc)
                : CommandLegacy(name, label, desc, 0) {}
            ImVec4 GetState() const { return m_State; }
            void SetState(ImVec4 s) { m_State = s; MarkDirty(); }
            void OnCall() override {}
            void SaveState(nlohmann::json& v) override { v = {m_State.x, m_State.y, m_State.z, m_State.w}; }
            void LoadState(nlohmann::json& v) override
            {
                if (v.is_array()) { auto a = v.get<std::array<float, 4>>(); m_State = {a[0], a[1], a[2], a[3]}; }
            }
        };

        static CommandSpawnColour s_SpawnPrimary{
            "spawnprimarycolour",
            "Primary Colour",
            "The primary colour applied to vehicles spawned via Stand when \"Colour Spawned Vehicles\" is enabled."
        };

        static CommandSpawnColour s_SpawnSecondary{
            "spawnsecondarycolour",
            "Secondary Colour",
            "The secondary colour applied to vehicles spawned via Stand when \"Colour Spawned Vehicles\" is enabled."
        };
    }

    ImVec4 GetSpawnPrimaryColour()
    {
        return s_SpawnPrimary.GetState();
    }

    ImVec4 GetSpawnSecondaryColour()
    {
        return s_SpawnSecondary.GetState();
    }
}
