#pragma once
#include "Commands/Vehicle/Boost/CommandBoostShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandBoostCharge : public CommandSlider
        {
            bool m_ticking    = false;
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
    }
}
