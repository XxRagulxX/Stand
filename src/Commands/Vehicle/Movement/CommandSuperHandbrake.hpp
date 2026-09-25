#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandSuperHandbrake : public CommandToggle
        {
        public:
            explicit CommandSuperHandbrake(CommandList* parent)
                : CommandToggle(parent, LIT("Super Handbrake"), CMDNAMES("superhandbrake", "superhandbreak")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    if (PAD::IS_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_HANDBRAKE))
                    {
                        int veh = MovVehicle();
                        if (veh) ENTITY::SET_ENTITY_VELOCITY(veh, 0.f, 0.f, 0.f);
                    }
                    return true;
                });
            }
        };
    }
}
