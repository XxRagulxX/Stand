#pragma once
#include "Commands/Vehicle/Collisions/CommandCollisionsShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandVehRamp : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandVehRamp(CommandList* parent)
                : CommandSlider(parent, LIT("Ramp"), CMDNAMES("vehramp"), NOLABEL, 0, 2, 0) {}

            ~CommandVehRamp() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Enabled";
                case 2: return "Disabled";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = CollVehicle();
                if (!veh) return;
                void* cveh = Pointers.HandleToPtr(veh);
                if (!cveh) return;
                void* model = CollFieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    CollFieldAt<uint8_t>(model, 0x57C + 16) |= static_cast<uint8_t>(1u << 1);
                else
                    CollFieldAt<uint8_t>(model, 0x57C + 16) &= static_cast<uint8_t>(~(1u << 1));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
                }
            }
        };
    }
}
