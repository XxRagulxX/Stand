#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include <unordered_map>

namespace Stand
{
    namespace
    {
        class CommandHeliAutoStabilisation : public CommandToggle
        {
            struct ModValues
            {
                float orig_thrust;
                float orig_attack_lift;
                float orig_attack_dive;
                float orig_pitch;
                float orig_roll;
                float orig_yaw;
                float orig_wind;
                float mod_thrust;
                float mod_pitch;
                float mod_roll;
                float mod_yaw;
            };
            std::unordered_map<int, ModValues> m_saved;

        public:
            explicit CommandHeliAutoStabilisation(CommandList* parent)
                : CommandToggle(parent, LIT("Disable Heli Auto-Stabilisation"), CMDNAMES("disableheliautostabilisation", "heliautostabilisation")) {}

            void onChange(Click& click) override
            {
                if (!m_on)
                {
                    click.ensureScriptThread([this] { RestoreAll(); });
                    return;
                }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on)
                    {
                        RestoreAll();
                        return false;
                    }
                    int veh = MovVehicle();
                    if (!veh || !VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)))
                        return true;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return true;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return true;
                    void* fly = FindSubHandling(hdata, 1);
                    if (!fly) fly = FindSubHandling(hdata, 2);
                    if (!fly) return true;

                    if (m_saved.find(veh) == m_saved.end())
                    {
                        float t = FieldAt<float>(fly, 0x08);
                        float p = FieldAt<float>(fly, 0x3C);
                        float r = FieldAt<float>(fly, 0x30);
                        float y = FieldAt<float>(fly, 0x20);
                        m_saved[veh] = {
                            t,
                            FieldAt<float>(fly, 0x48),
                            FieldAt<float>(fly, 0x4C),
                            p, r, y,
                            FieldAt<float>(fly, 0x58),
                            t * 1.35f,
                            p * 0.01f,
                            r * 0.01f,
                            y * 0.01f,
                        };
                    }

                    const auto& mv = m_saved[veh];
                    FieldAt<float>(fly, 0x08) = mv.mod_thrust;
                    FieldAt<float>(fly, 0x48) = 0.f;
                    FieldAt<float>(fly, 0x4C) = 0.f;
                    FieldAt<float>(fly, 0x3C) = mv.mod_pitch;
                    FieldAt<float>(fly, 0x30) = mv.mod_roll;
                    FieldAt<float>(fly, 0x20) = mv.mod_yaw;
                    FieldAt<float>(fly, 0x58) = 0.f;
                    return true;
                });
            }

        private:
            void RestoreAll()
            {
                for (auto& [veh, mv] : m_saved)
                {
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) continue;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) continue;
                    void* fly = FindSubHandling(hdata, 1);
                    if (!fly) fly = FindSubHandling(hdata, 2);
                    if (!fly) continue;
                    FieldAt<float>(fly, 0x08) = mv.orig_thrust;
                    FieldAt<float>(fly, 0x48) = mv.orig_attack_lift;
                    FieldAt<float>(fly, 0x4C) = mv.orig_attack_dive;
                    FieldAt<float>(fly, 0x3C) = mv.orig_pitch;
                    FieldAt<float>(fly, 0x30) = mv.orig_roll;
                    FieldAt<float>(fly, 0x20) = mv.orig_yaw;
                    FieldAt<float>(fly, 0x58) = mv.orig_wind;
                }
                m_saved.clear();
            }
        };
    }
}
