#include "Commands/Vehicle/Boost/CommandListRocketBoost.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        static int BoostVehicle()
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
            veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
            return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
        }

        template<typename T>
        static T& FieldAt(void* base, size_t offset)
        {
            return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
        }

        static void* GetVehPtr(int veh)
        {
            return Pointers.HandleToPtr(veh);
        }

        class CommandRocketOverride : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandRocketOverride(CommandList* parent)
                : CommandSlider(parent, LIT("Override"), CMDNAMES("vehrocket"),
                    LIT("Allows you to change whether the vehicles you drive have rocket boost."),
                    0, 2, 0) {}

            ~CommandRocketOverride() override
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
                    FieldAt<uint8_t>(model, 0x57C + 15) |= static_cast<uint8_t>(1u << 6);
                else
                    FieldAt<uint8_t>(model, 0x57C + 15) &= static_cast<uint8_t>(~(1u << 6));
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

        class CommandBoostCharge : public CommandSlider
        {
            bool m_ticking   = false;
            bool m_was_active = false;
        public:
            explicit CommandBoostCharge(CommandList* parent)
                : CommandSlider(parent, LIT("Charge Behaviour"), CMDNAMES("boostcharge"),
                    NOLABEL, 0, 4, 0) {}

            ~CommandBoostCharge() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Infinite";
                case 2: return "Instant Refill";
                case 3: return "No Refill Time";
                case 4: return "Always Empty";
                default: return "Normal";
                }
            }

            void onTick() override
            {
                int veh = BoostVehicle();
                if (!veh) return;
                switch (value)
                {
                case 1:
                    VEHICLE::SET_ROCKET_BOOST_FILL(veh, 1.f);
                    break;
                case 2:
                    if (VEHICLE::IS_ROCKET_BOOST_ACTIVE(veh))
                        m_was_active = true;
                    else if (m_was_active)
                    {
                        VEHICLE::SET_ROCKET_BOOST_FILL(veh, 1.f);
                        m_was_active = false;
                    }
                    break;
                case 3:
                    VEHICLE::SET_SCRIPT_ROCKET_BOOST_RECHARGE_TIME(veh, 0.f);
                    break;
                case 4:
                    {
                        void* cveh = GetVehPtr(veh);
                        if (cveh)
                        {
                            FieldAt<float>(cveh, 0x0300) = 0.f;
                            FieldAt<float>(cveh, 0x0304) = 0.f;
                        }
                    }
                    break;
                }
            }

            void onChange(Click& click, int prev) override
            {
                DoCleanup(prev, click);

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
                    if (value == 2)
                    {
                        click.ensureScriptThread([] {
                            int veh = BoostVehicle();
                            if (veh && !VEHICLE::IS_ROCKET_BOOST_ACTIVE(veh))
                            {
                                void* cveh = GetVehPtr(veh);
                                if (cveh) FieldAt<float>(cveh, 0x0300) = 1.25f;
                            }
                        });
                    }
                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                }
            }

        private:
            void DoCleanup(int prev, Click& click)
            {
                m_was_active = false;
                if (prev == 1)
                {
                    click.ensureScriptThread([] {
                        int veh = BoostVehicle();
                        if (veh) VEHICLE::SET_ROCKET_BOOST_FILL(veh, 0.f);
                    });
                }
                else if (prev == 3 || prev == 4)
                {
                    click.ensureScriptThread([] {
                        int veh = BoostVehicle();
                        if (!veh) return;
                        void* cveh = GetVehPtr(veh);
                        if (cveh) FieldAt<float>(cveh, 0x0304) = 0.5f;
                    });
                }
            }
        };

        class CommandAutoBoostToggle : public CommandToggle
        {
        public:
            explicit CommandAutoBoostToggle(CommandList* parent)
                : CommandToggle(parent, LIT("Auto Rocket Boost"), CMDNAMES("autoboost"),
                    LIT("Automatically uses the rocket boost when it's charged.")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = BoostVehicle();
                    if (veh) VEHICLE::SET_ROCKET_BOOST_ACTIVE(veh, true);
                    return true;
                });
            }
        };
    }

    CommandListRocketBoost::CommandListRocketBoost(CommandList* parent)
        : CommandList(parent, LIT("Rocket Boost"), CMDNAMES_0())
    {
        createChild<CommandRocketOverride>();
        createChild<CommandVerticalRocketBoost>();
        createChild<CommandBoostCharge>();
        createChild<CommandAutoBoostToggle>();
    }
}
