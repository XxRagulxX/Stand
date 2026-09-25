#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandLessenTyreBurnouts : public CommandToggle
        {
            float m_saved = 1.f;
            bool m_applied = false;

        public:
            explicit CommandLessenTyreBurnouts(CommandList* parent)
                : CommandToggle(parent, LIT("Lessen Tyre Burnouts"), CMDNAMES("launchcontrol", "lessentyreburnouts", "lessenburnouts")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;
                    float& mult = FieldAt<float>(hdata, 0x0A8);
                    if (m_on)
                    {
                        if (mult != 0.f) m_saved = mult;
                        mult = 0.f;
                        m_applied = true;
                    }
                    else if (m_applied)
                    {
                        mult = m_saved;
                        m_saved = 1.f;
                        m_applied = false;
                    }
                });
            }
        };
    }
}
