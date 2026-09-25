#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

#include <cstdint>
#include <string>

namespace Stand
{
    namespace
    {
        class CommandParachuteEnable : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandParachuteEnable(CommandList* parent)
                : CommandSlider(parent, LIT("Enable Parachute"), CMDNAMES("vehparachute"), NOLABEL, 0, 2, 0) {}

            ~CommandParachuteEnable() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "On";
                case 2: return "Off";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh) return;
                void* cveh = Pointers.HandleToPtr(veh);
                if (!cveh) return;
                void* model = *reinterpret_cast<void**>(reinterpret_cast<char*>(cveh) + 0x20);
                if (!model) return;
                uint8_t& b = *reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(model) + 0x57C + 16);
                if (value == 1)
                    b |= 0x01u;
                else
                    b &= static_cast<uint8_t>(~0x01u);
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

        struct ParachuteVariant { const char* name; uint32_t hash; int tint; };
        static constexpr ParachuteVariant kParachuteVariants[] = {
            { "General: Camo",             "sr_prop_specraces_para_s_01"_J, 0 },
            { "General: Red",              "sr_prop_specraces_para_s_01"_J, 1 },
            { "General: White/Blue/Yellow","sr_prop_specraces_para_s_01"_J, 2 },
            { "General: Lilac/Red/White",  "sr_prop_specraces_para_s_01"_J, 3 },
            { "General: Red/White/Blue",   "sr_prop_specraces_para_s_01"_J, 4 },
            { "General: Blue",             "sr_prop_specraces_para_s_01"_J, 5 },
            { "General: Lilac",            "sr_prop_specraces_para_s_01"_J, 6 },
            { "General: Lilac/Yellow",     "sr_prop_specraces_para_s_01"_J, 7 },
            { "General: Yellow/Blue/White","prop_v_parachute"_J,            0 },
            { "General: Orange",           "gr_prop_gr_para_s_01"_J,        0 },
            { "General: White",            "p_parachute1_sp_dec"_J,         0 },
            { "SecuroServ",                "imp_prop_impexp_para_s"_J,      0 },
            { "Broken",                    "p_para_broken1_s"_J,            0 },
        };

        class CommandParachuteModel : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandParachuteModel(CommandList* parent)
                : CommandSlider(parent, LIT("Appearance"), CMDNAMES("vehparamodel"), NOLABEL, 0, 12, 11) {}

            ~CommandParachuteModel() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                return kParachuteVariants[value].name;
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh) return;
                VEHICLE::VEHICLE_SET_PARACHUTE_MODEL_OVERRIDE(veh, kParachuteVariants[value].hash);
                VEHICLE::VEHICLE_SET_PARACHUTE_MODEL_TINT_INDEX(veh, kParachuteVariants[value].tint);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
            }
        };
    }
}
