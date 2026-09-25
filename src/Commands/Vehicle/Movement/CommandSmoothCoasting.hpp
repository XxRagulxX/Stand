#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandSmoothCoasting : public CommandToggle
        {
            uint32_t m_saved_flags = 0;
            bool m_applied = false;

        public:
            explicit CommandSmoothCoasting(CommandList* parent)
                : CommandToggle(parent, LIT("Smooth Coasting"), CMDNAMES("smoothcoasting"),
                    LIT("Disables engine braking when no throttle is applied.")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;
                    uint32_t& flags = FieldAt<uint32_t>(hdata, 0x128);
                    if (m_on)
                    {
                        m_saved_flags = flags;
                        flags |= (1u << 8);
                        m_applied = true;
                    }
                    else if (m_applied)
                    {
                        flags = m_saved_flags;
                        m_applied = false;
                    }
                });
            }
        };
    }
}
