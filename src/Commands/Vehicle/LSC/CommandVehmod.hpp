#pragma once
#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    namespace
    {
        class CommandVehmodInt : public CommandSlider
        {
            const int m_mod;
        public:
            explicit CommandVehmodInt(CommandList* parent, int modType)
                : CommandSlider(parent, LIT(Mod::GetName(modType)), CMDNAMES_0(), NOLABEL, -1, -1, -1)
                , m_mod(modType)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehmodInt() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                int veh = LscGetVehicle();
                s_lsc_in_veh.store(veh != 0, std::memory_order_relaxed);

                if (!veh) { setMaxValue(min_value); flags |= CMDFLAG_CONCEALED; return; }
                int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, m_mod) - 1;
                setMaxValue(mx);
                bool na = (mx < 0);
                if (na && !s_lsc_show_na.load(std::memory_order_relaxed))
                    flags |= CMDFLAG_CONCEALED;
                else
                    flags &= ~CMDFLAG_CONCEALED;
                if (!na) {
                    Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    setValueIndicator(c, VEHICLE::GET_VEHICLE_MOD(veh, m_mod));
                }
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                int v = value, mt = m_mod;
                click.ensureScriptThread([v, mt] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::SET_VEHICLE_MOD(veh, mt, v, false);
                });
            }
        };

        class CommandVehmodBool : public CommandToggle
        {
            const int m_mod;
        public:
            explicit CommandVehmodBool(CommandList* parent, int modType, std::vector<CommandName> cmds = {})
                : CommandToggle(parent, LIT(Mod::GetName(modType)), std::move(cmds))
                , m_mod(modType)
            {}

            void onChange(Click& click) override
            {
                if (click.isAuto()) return;
                bool on = m_on; int mt = m_mod;
                click.ensureScriptThread([on, mt] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, mt, on);
                });
            }
        };
    }
}
