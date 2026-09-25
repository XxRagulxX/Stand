#pragma once
#include "Commands/Vehicle/Boost/CommandBoostShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandVerticalRocketBoost : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandVerticalRocketBoost(CommandList* parent)
                : CommandSlider(parent, LIT("Vertical Rocket Boost"), CMDNAMES("verticalboost"),
                    NOLABEL, 0, 2, 0) {}

            ~CommandVerticalRocketBoost() override
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
                int veh = BoostVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                void* model = FieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    FieldAt<uint8_t>(model, 0x57C + 20) |= static_cast<uint8_t>(1u << 1);
                else
                    FieldAt<uint8_t>(model, 0x57C + 20) &= static_cast<uint8_t>(~(1u << 1));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking)
                    {
                        CommandTickDispatch::RemoveCommand(this);
                        m_ticking = false;
                    }
                }
                else
                {
                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                }
            }
        };
    }
}
