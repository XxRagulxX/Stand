#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include <unordered_map>

namespace Stand
{
    namespace
    {
        class CommandDriftMode : public CommandToggle
        {
            std::unordered_map<uintptr_t, float> m_saved_floats;
            std::unordered_map<uintptr_t, uint32_t> m_saved_flags;
            bool m_applied = false;

            template<typename T>
            void SaveAndSet(void* base, size_t off, T val,
                std::unordered_map<uintptr_t, T>& store)
            {
                uintptr_t key = reinterpret_cast<uintptr_t>(base) + off;
                T& field = FieldAt<T>(base, off);
                store.emplace(key, field);
                field = val;
            }

        public:
            explicit CommandDriftMode(CommandList* parent)
                : CommandToggle(parent, LIT("Drift Mode"), CMDNAMES("driftmode")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;

                    if (m_on)
                    {
                        if (m_applied) return;
                        m_applied = true;

                        static constexpr uint32_t kDriftFlags =
                            (1u<<4)|(1u<<6)|(1u<<8)|(1u<<9)|(1u<<13)|(1u<<15)|(1u<<18)|(1u<<20)|(1u<<22);

                        void* car_sub = FindSubHandling(hdata, 8);
                        if (car_sub)
                            SaveAndSet(car_sub, 0x3C, kDriftFlags, m_saved_flags);

                        float speed = ENTITY::GET_ENTITY_SPEED(veh);
                        float base_min = 1.625f, base_max = 1.63f;
                        if (speed < 4.5f)       { base_min *= 1.50f; base_max *= 1.50f; }
                        else if (speed < 8.f)   { base_min *= 1.30f; base_max *= 1.30f; }
                        else if (speed < 13.f)  { base_min *= 1.15f; base_max *= 1.15f; }

                        SaveAndSet(hdata, 0x04C, 1.65f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0BC, 2.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0D0, 0.025f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C0, 0.1f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0B8, 1.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x058, 4.8f,     m_saved_floats);
                        SaveAndSet(hdata, 0x05C, 4.7f,     m_saved_floats);
                        SaveAndSet(hdata, 0x090, base_min, m_saved_floats);
                        SaveAndSet(hdata, 0x088, base_max, m_saved_floats);
                        SaveAndSet(hdata, 0x044, 1.1764f,  m_saved_floats);
                        SaveAndSet(hdata, 0x048, 0.35f,    m_saved_floats);
                        SaveAndSet(hdata, 0x054, 1.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x060, 0.7059f,  m_saved_floats);
                        SaveAndSet(hdata, 0x064, 44.1666f, m_saved_floats);
                        SaveAndSet(hdata, 0x068, 36.8055f, m_saved_floats);
                        SaveAndSet(hdata, 0x06C, 0.8750f,  m_saved_floats);
                        SaveAndSet(hdata, 0x074, 0.64f,    m_saved_floats);
                        SaveAndSet(hdata, 0x078, 1.36f,    m_saved_floats);
                        SaveAndSet(hdata, 0x07C, 0.9f,     m_saved_floats);
                        SaveAndSet(hdata, 0x080, 1.3482f,  m_saved_floats);
                        SaveAndSet(hdata, 0x084, 0.7416f,  m_saved_floats);
                        SaveAndSet(hdata, 0x08C, 0.6535f,  m_saved_floats);
                        SaveAndSet(hdata, 0x098, 0.5139f,  m_saved_floats);
                        SaveAndSet(hdata, 0x09C, 1.9455f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A0, 0.1500f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A4, 6.6666f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A8, 0.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0AC, 0.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0B0, 0.9240f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0B4, 1.076f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C4, 0.155f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C8, 0.0675f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0CC, -0.1f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0D4, 1.04f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0D8, 0.96f,    m_saved_floats);

                        VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 30.0f);
                    }
                    else if (m_applied)
                    {
                        for (auto& [key, val] : m_saved_floats)
                            *reinterpret_cast<float*>(key) = val;
                        for (auto& [key, val] : m_saved_flags)
                            *reinterpret_cast<uint32_t*>(key) = val;
                        m_saved_floats.clear();
                        m_saved_flags.clear();
                        m_applied = false;
                        VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 10.0f);
                    }
                });
            }
        };
    }
}
