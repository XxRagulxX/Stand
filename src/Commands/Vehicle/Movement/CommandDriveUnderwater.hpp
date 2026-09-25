#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandDriveUnderwater : public CommandToggle
        {
        public:
            explicit CommandDriveUnderwater(CommandList* parent)
                : CommandToggle(parent, LIT("Drive Underwater"), CMDNAMES("driveunderwater", "waterwheels", "waterdrive")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return true;
                    unsigned int vtype = FieldAt<unsigned int>(cveh, 0x0C28);
                    if (vtype == 13 || vtype == 15) return true;
                    void* ph_inst = FieldAt<void*>(cveh, 0x30);
                    if (!ph_inst) return true;
                    void* archetype = FieldAt<void*>(ph_inst, 0x10);
                    if (!archetype) return true;
                    FieldAt<float>(archetype, 0x54) = 0.f;
                    return true;
                });
            }
        };
    }
}
