#include "Commands/Vehicle/AutoDrive/CommandListAutoDrive.hpp"

#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Game/eDrivingFlags.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"

#include <cfloat>
#include <cmath>
#include <string>
#include <vector>

namespace Stand
{
    namespace
    {
        struct AutoDriveCoord
        {
            float x = 0.f, y = 0.f, z = 0.f;
            bool valid = false;

            void reset() { valid = false; x = y = z = 0.f; }

            float distanceTopdown(float ox, float oy) const
            {
                float dx = x - ox, dy = y - oy;
                return std::sqrtf(dx * dx + dy * dy);
            }
        };

        static constexpr float kDefaultCruiseSpeed = 12.f;

        static AutoDriveCoord findBlipCoord(int sprite)
        {
            Blip blip = HUD::GET_FIRST_BLIP_INFO_ID(sprite);
            AutoDriveCoord best;
            float bestDist = FLT_MAX;
            int ped = PLAYER::GET_PLAYER_PED(-1);
            Vector3 myPos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);

            while (HUD::DOES_BLIP_EXIST(blip))
            {
                Vector3 coord = HUD::GET_BLIP_INFO_ID_COORD(blip);
                float dx = coord.x - myPos.x, dy = coord.y - myPos.y;
                float dist = std::sqrtf(dx * dx + dy * dy);
                if (dist < bestDist)
                {
                    bestDist = dist;
                    best = { coord.x, coord.y, coord.z, true };
                }
                blip = HUD::GET_NEXT_BLIP_INFO_ID(sprite);
            }
            return best;
        }

        static AutoDriveCoord findWaypointCoord()
        {
            if (!HUD::IS_WAYPOINT_ACTIVE())
                return {};
            int sprite = HUD::GET_WAYPOINT_BLIP_ENUM_ID();
            Blip blip = HUD::GET_FIRST_BLIP_INFO_ID(sprite);
            if (HUD::DOES_BLIP_EXIST(blip))
            {
                Vector3 coord = HUD::GET_BLIP_INFO_ID_COORD(blip);
                return { coord.x, coord.y, coord.z, true };
            }
            return {};
        }

        class CommandDrivingStyle : public CommandSlider
        {
            static constexpr const char* kNames[] = {
                "Lawful", "Run Lights", "Aggressive", "Backwards"
            };
            static constexpr int kFlags[] = {
                (int)eDrivingFlags::CUSTOM_Lawful,
                (int)eDrivingFlags::CUSTOM_Lawful_RunLights,
                (int)eDrivingFlags::CUSTOM_Aggressive,
                (int)eDrivingFlags::CUSTOM_Backwards,
            };

        public:
            explicit CommandDrivingStyle(CommandList* parent)
                : CommandSlider(parent, LIT("Driving Style"), CMDNAMES("autodrivestyle"), NOLABEL, 0, 3, 0) {}

            std::string getValueText() const override { return kNames[value]; }

            int getFlags() const { return kFlags[value]; }
        };

        class CommandAutoDriveTicker : public CommandPhysical
        {
        public:
            AutoDriveCoord m_dest;
            int m_veh = 0;
            bool m_active = false;
            bool m_ticking = false;
            time_t m_last_speed = 0;

            CommandSliderFloat* m_speed = nullptr;
            CommandDrivingStyle* m_style = nullptr;
            CommandToggle* m_auto_kill = nullptr;
            CommandToggle* m_noclip = nullptr;

            explicit CommandAutoDriveTicker(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED) {}

            ~CommandAutoDriveTicker() override
            {
                if (m_ticking)
                    CommandTickDispatch::RemoveCommand(this);
            }

            void start(float x, float y, float z, bool has_dest)
            {
                int ped = PLAYER::GET_PLAYER_PED(-1);
                m_dest = { x, y, z, has_dest };
                m_veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                m_last_speed = 0;

                int flags = m_style ? m_style->getFlags() : (int)eDrivingFlags::CUSTOM_Lawful;
                TASK::CLEAR_PED_TASKS(ped);

                if (!has_dest)
                    TASK::TASK_VEHICLE_DRIVE_WANDER(ped, m_veh, kDefaultCruiseSpeed, flags);
                else
                    TASK::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(ped, m_veh, x, y, z, kDefaultCruiseSpeed, flags, 0.f);

                if (!m_ticking)
                {
                    CommandTickDispatch::AddCommand(this);
                    m_ticking = true;
                }
                m_active = true;
            }

            void stop()
            {
                if (!m_active && !m_ticking) return;
                int ped = PLAYER::GET_PLAYER_PED(-1);
                TASK::CLEAR_PED_TASKS(ped);
                m_dest.reset();
                m_veh = 0;
                m_active = false;
                m_last_speed = 0;
                if (m_ticking)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            }

            void onTick() override
            {
                if (!m_active) return;

                int ped = PLAYER::GET_PLAYER_PED(-1);
                int cur_veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);

                if (!cur_veh || cur_veh != m_veh || !ENTITY::DOES_ENTITY_EXIST(m_veh))
                {
                    stop();
                    return;
                }

                if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(m_veh, -1, FALSE) != ped)
                {
                    stop();
                    return;
                }

                if (m_auto_kill && m_auto_kill->m_on)
                {
                    bool pressing =
                        PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_ACCELERATE) ||
                        PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_BRAKE) ||
                        PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_HANDBRAKE) ||
                        PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_MOVE_LEFT_ONLY) ||
                        PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_MOVE_RIGHT_ONLY);
                    if (pressing)
                    {
                        stop();
                        return;
                    }
                }

                if (m_noclip && m_noclip->m_on)
                {
                    for (auto v : Pools::GetVehicles())
                    {
                        int h = v.GetHandle();
                        if (h && h != m_veh)
                            ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(h, m_veh, TRUE);
                    }
                }

                if (m_dest.valid)
                {
                    Vector3 myPos = ENTITY::GET_ENTITY_COORDS(m_veh, TRUE);
                    float dist = m_dest.distanceTopdown(myPos.x, myPos.y);

                    if (dist < 40.f)
                    {
                        FiberPool::queueJob([veh = m_veh] {
                            VEHICLE::BRING_VEHICLE_TO_HALT(veh, 10.f, 2000, FALSE);
                            Script::current()->yield(2000);
                            VEHICLE::STOP_BRINGING_VEHICLE_TO_HALT(veh);
                        });
                        stop();
                        return;
                    }

                    if (dist < 70.f)
                    {
                        TASK::SET_DRIVE_TASK_CRUISE_SPEED(ped, kDefaultCruiseSpeed);
                        return;
                    }
                }

                if (GET_MILLIS_SINCE(m_last_speed) > 1000)
                {
                    float spd = kDefaultCruiseSpeed;
                    if (m_speed)
                    {
                        float override_spd = m_speed->getFloatValue();
                        if (override_spd > 0.f)
                        {
                            spd = override_spd;
                        }
                        else if (m_style && m_style->value == 2)
                        {
                            spd = 80.f;
                        }
                        else
                        {
                            std::vector<float> speeds;
                            for (auto v : Pools::GetVehicles())
                            {
                                int h = v.GetHandle();
                                if (!h || h == m_veh) continue;
                                float vs = ENTITY::GET_ENTITY_SPEED(h);
                                Vector3 myPos = ENTITY::GET_ENTITY_COORDS(m_veh, TRUE);
                                Vector3 their = ENTITY::GET_ENTITY_COORDS(h, TRUE);
                                if (vs > kDefaultCruiseSpeed && std::fabsf(myPos.z - their.z) <= 3.f)
                                    speeds.push_back(vs);
                            }
                            if (!speeds.empty())
                            {
                                float avg = 0.f;
                                for (float s : speeds) avg += s;
                                spd = avg / (float)speeds.size();
                            }
                        }
                    }
                    TASK::SET_DRIVE_TASK_CRUISE_SPEED(ped, spd);
                    TASK::SET_DRIVE_TASK_MAX_CRUISE_SPEED(ped, spd, FALSE);
                    m_last_speed = get_current_time_millis();
                }
            }
        };

        class CommandAutoDriveDest : public CommandPhysical
        {
            int m_sprite;
            bool m_is_waypoint;
            CommandAutoDriveTicker* const m_ticker;

        public:
            explicit CommandAutoDriveDest(CommandList* parent, Label&& name,
                std::vector<CommandName> cmdnames, int sprite, bool is_waypoint,
                CommandAutoDriveTicker* ticker)
                : CommandPhysical(COMMAND_ACTION, parent, std::move(name), std::move(cmdnames))
                , m_sprite(sprite), m_is_waypoint(is_waypoint), m_ticker(ticker) {}

            void onClick(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
                    if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, FALSE) != ped) return;

                    Hash model = ENTITY::GET_ENTITY_MODEL(veh);
                    if (VEHICLE::IS_THIS_MODEL_A_PLANE(model) || VEHICLE::IS_THIS_MODEL_AN_AMPHIBIOUS_CAR(model))
                        return;

                    if (m_sprite == -1)
                    {
                        m_ticker->start(0.f, 0.f, 0.f, false);
                        return;
                    }

                    AutoDriveCoord coord = m_is_waypoint
                        ? findWaypointCoord()
                        : findBlipCoord(m_sprite);

                    if (!coord.valid) return;

                    Vector3 myPos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                    float dx = coord.x - myPos.x, dy = coord.y - myPos.y;
                    if (std::sqrtf(dx * dx + dy * dy) < 40.f) return;

                    m_ticker->start(coord.x, coord.y, coord.z, true);
                });
            }
        };

        class CommandListDriveTo : public CommandList
        {
        public:
            explicit CommandListDriveTo(CommandList* parent, CommandAutoDriveTicker* ticker)
                : CommandList(parent, LIT("Drive To..."), CMDNAMES("goto"))
            {
                createChild<CommandAutoDriveDest>(LIT("Wherever The Wind Takes Us"), CMDNAMES("gotonowhere"),
                    -1, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Waypoint"), CMDNAMES("gotowaypoint"),
                    8, true, ticker);
                createChild<CommandAutoDriveDest>(LIT("Casino"), CMDNAMES("gotocasino"),
                    679, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Ammu-Nation"), CMDNAMES("gotoammu"),
                    110, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Clothes Store"), CMDNAMES("gotoclothes"),
                    73, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Tattoo Parlor"), CMDNAMES("gototattoo"),
                    75, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Barber Shop"), CMDNAMES("gotobarber"),
                    71, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Los Santos Customs"), CMDNAMES("gotolsc"),
                    72, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Movies"), CMDNAMES("gotomovies"),
                    135, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Store"), CMDNAMES("gotostore"),
                    52, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Motorcycle Gang Clubhouse"), CMDNAMES("gotoclubhouse"),
                    492, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Facility"), CMDNAMES("gotofacility"),
                    590, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Agency"), CMDNAMES("gotoagency"),
                    826, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Office"), CMDNAMES("gotooffice"),
                    475, false, ticker);
                createChild<CommandAutoDriveDest>(LIT("Hangar"), CMDNAMES("gotohangar"),
                    569, false, ticker);
            }
        };

        class CommandAutoDriveCancel : public CommandPhysical
        {
            CommandAutoDriveTicker* const m_ticker;

        public:
            explicit CommandAutoDriveCancel(CommandList* parent, CommandAutoDriveTicker* ticker)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Cancel"), CMDNAMES("stopautodrive"))
                , m_ticker(ticker) {}

            void onClick(Click& click) override
            {
                click.ensureScriptThread([this] {
                    m_ticker->stop();
                });
            }
        };
    }

    CommandListAutoDrive::CommandListAutoDrive(CommandList* parent)
        : CommandList(parent, LIT("Auto-Drive"), CMDNAMES("autodrive"))
    {
        auto ticker_ptr = makeChild<CommandAutoDriveTicker>();
        CommandAutoDriveTicker* ticker = ticker_ptr.get();
        children.emplace_back(std::move(ticker_ptr));

        createChild<CommandListDriveTo>(ticker);
        createChild<CommandAutoDriveCancel>(ticker);

        auto speed = makeChild<CommandSliderFloat>(LIT("Cruise Speed"), CMDNAMES("autodrivespeed"),
            LIT("Speed in m/s. Set to 0 for auto (matches traffic speed)."), 0, 8000, 0, 100);
        auto style = makeChild<CommandDrivingStyle>();
        auto beacon = makeChild<CommandToggle>(LIT("Show Destination Beacon"), CMDNAMES("autodrivebeacon"));
        auto auto_kill = makeChild<CommandToggle>(LIT("Cancel On Input"), CMDNAMES("autodriveautocancel"),
            LIT("Cancels Auto-Drive when you press a movement input."));
        auto noclip = makeChild<CommandToggle>(LIT("No Vehicle Collision While Auto-Driving"),
            CMDNAMES("noclipwhileautodrive"),
            LIT("Disables collisions with other vehicles while Auto-Drive is active."));

        ticker->m_speed     = speed.get();
        ticker->m_style     = style.get();
        ticker->m_auto_kill = auto_kill.get();
        ticker->m_noclip    = noclip.get();

        children.emplace_back(std::move(speed));
        children.emplace_back(std::move(style));
        children.emplace_back(std::move(beacon));
        children.emplace_back(std::move(auto_kill));
        children.emplace_back(std::move(noclip));
    }
}
