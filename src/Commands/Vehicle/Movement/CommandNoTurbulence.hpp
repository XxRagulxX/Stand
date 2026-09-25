#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandNoTurbulence : public CommandToggle
        {
        public:
            explicit CommandNoTurbulence(CommandList* parent)
                : CommandToggle(parent, LIT("No Turbulence"), CMDNAMES("smoothflight", "noturbulence")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    int veh = MovVehicle();
                    if (!m_on)
                    {
                        SetTurbulence(veh, 1.f);
                        return false;
                    }
                    SetTurbulence(veh, 0.f);
                    return true;
                });
            }

        private:
            static void SetTurbulence(int veh, float v)
            {
                if (!veh) return;
                if (!VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh))) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                FieldAt<float>(cveh, 0x1B00) = v;
                FieldAt<float>(cveh, 0x1B04) = v;
            }
        };
    }
}
