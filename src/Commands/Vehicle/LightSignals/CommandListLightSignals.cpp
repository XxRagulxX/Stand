#include "Commands/Vehicle/LightSignals/CommandListLightSignals.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"

#include <cmath>
#include <string>

namespace Stand
{
    namespace
    {
        class CommandTurnSignal : public CommandSlider
        {
            static constexpr const char* kNames[] = { "Off", "Left", "Right", "Hazard" };

            enum : int { OFF = 0, LEFT = 1, RIGHT = 2, HAZARD = 3 };

            bool m_ticking = false;
            float m_last_heading = -1.f;
            int m_last_veh = 0;

        public:
            CommandToggle* automatic = nullptr;

            explicit CommandTurnSignal(CommandList* parent)
                : CommandSlider(parent, LIT("Turn Signal"), CMDNAMES("signal", "blinker"), NOLABEL, 0, 3, OFF) {}

            std::string getValueText() const override { return kNames[value]; }

            void onChange(Click& click, int /*prev_value*/) override
            {
                if (click.isAuto()) return;
                click.ensureScriptThread([this] {
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
                    {
                        value = OFF;
                        return;
                    }

                    applyToVehicle(veh);

                    if (value == LEFT || value == RIGHT)
                        m_last_heading = ENTITY::GET_ENTITY_HEADING(veh);
                    else
                        m_last_heading = -1.f;

                    m_last_veh = veh;

                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                });
            }

            ~CommandTurnSignal() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int ped = PLAYER::GET_PLAYER_PED(-1);
                int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);

                if (m_last_veh && veh != m_last_veh)
                {
                    clearVehicle(m_last_veh);
                    m_last_veh = 0;
                    m_last_heading = -1.f;
                    setOff();
                }
                else if (!veh)
                {
                    m_last_veh = 0;
                    m_last_heading = -1.f;
                    setOff();
                }
                else if (automatic && automatic->m_on
                    && m_last_heading >= 0.f
                    && (value == LEFT || value == RIGHT))
                {
                    float cur = ENTITY::GET_ENTITY_HEADING(veh);
                    float diff = std::fabsf(cur - m_last_heading);
                    if (diff > 180.f) diff = 360.f - diff;
                    if (diff > 60.f)
                    {
                        clearVehicle(veh);
                        m_last_heading = -1.f;
                        m_last_veh = 0;
                        setOff();
                    }
                }

                if (value == OFF)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            }

        private:
            void applyToVehicle(int veh)
            {
                VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 0, value == LEFT  || value == HAZARD);
                VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 1, value == RIGHT || value == HAZARD);
            }

            void clearVehicle(int veh)
            {
                VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 0, FALSE);
                VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 1, FALSE);
            }

            void setOff()
            {
                value = OFF;
            }
        };

        class CommandUseBrakelights : public CommandToggle
        {
        public:
            explicit CommandUseBrakelights(CommandList* parent)
                : CommandToggle(parent, LIT("Use Brake Lights When Stopped"), CMDNAMES("brakelights")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
                    if (!m_on) return false;
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                        VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(veh, ENTITY::GET_ENTITY_SPEED(veh) < 0.25f);
                    return m_on;
                });
            }
        };

        class CommandMuteSirens : public CommandToggle
        {
        public:
            explicit CommandMuteSirens(CommandList* parent)
                : CommandToggle(parent, LIT("Mute Sirens"), CMDNAMES("mutesirens", "sirensoff", "sirenoff"),
                      LIT("This will only affect your game.")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
                    if (!m_on) return false;
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (veh && ENTITY::DOES_ENTITY_EXIST(veh)
                        && VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, FALSE) == ped)
                    {
                        VEHICLE::SET_VEHICLE_HAS_MUTED_SIRENS(veh, TRUE);
                    }
                    return m_on;
                });
            }
        };
    }

    CommandListLightSignals::CommandListLightSignals(CommandList* parent)
        : CommandList(parent, LIT("Light Signals"), CMDNAMES("lightsignals"))
    {
        auto turn = createChild<CommandTurnSignal>();
        turn->automatic = createChild<CommandToggle>(
            LIT("Automatically Disable Turn Signals"), CMDNAMES("autosignal"));
        createChild<CommandUseBrakelights>();
        createChild<CommandMuteSirens>();
    }
}
