#include "Commands/Vehicle/Countermeasures/CommandListCountermeasures.hpp"

#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"

#include <cstdlib>
#include <string>
#include <vector>

namespace Stand
{
    namespace
    {
        class CommandCmPattern : public CommandSlider
        {
            static constexpr const char* kNames[] = {
                "Horizontal", "Horizontal, Downwards", "Horizontal, Upwards",
                "Vertical", "Vertical, Downwards", "Random"
            };
            static constexpr int kEnumMap[] = {0, 2, 1, 3, 4, 5};

        public:
            explicit CommandCmPattern(CommandList* parent)
                : CommandSlider(parent, LIT("Pattern"), CMDNAMES("flarepattern"), NOLABEL, 0, 5, 0) {}

            std::string getValueText() const override { return kNames[value]; }

            Vector3 getVec() const
            {
                switch (kEnumMap[value])
                {
                case 0: return {5.f, -5.f,  0.f};
                case 1: return {5.f, -5.f,  5.f};
                case 2: return {5.f, -5.f, -5.f};
                case 3: return {1.f, -1.f,  5.f};
                case 4: return {1.f, -1.f, -5.f};
                }
                return {
                    float(1 + rand() % 5),
                    float(-(1 + rand() % 5)),
                    float(rand() % 6) * (rand() % 2 ? 1.f : -1.f)
                };
            }
        };

        class CommandFireCm : public CommandPhysical
        {
        public:
            enum CM : uint8_t { CHAFF, FLARE, BOTH };

        private:
            bool m_chaff_busy = false;
            const CM m_cm;
            CommandSlider* const m_count;
            CommandToggle* const m_onlyair;
            CommandCmPattern* const m_pattern;

            [[nodiscard]] bool shouldDeploy(int veh) const
            {
                if (!m_onlyair->m_on) return true;
                Hash model = ENTITY::GET_ENTITY_MODEL(veh);
                return VEHICLE::IS_THIS_MODEL_A_PLANE(model)
                    || VEHICLE::IS_THIS_MODEL_A_HELI(model)
                    || model == MISC::GET_HASH_KEY("oppressor2");
            }

        public:
            explicit CommandFireCm(CommandList* parent, CM cm, Label&& name,
                std::vector<CommandName> cmdnames,
                CommandSlider* count, CommandToggle* onlyair, CommandCmPattern* pattern)
                : CommandPhysical(COMMAND_ACTION, parent, std::move(name), std::move(cmdnames))
                , m_cm(cm), m_count(count), m_onlyair(onlyair), m_pattern(pattern) {}

            void deployChaff(int veh)
            {
                if (m_chaff_busy) return;
                FiberPool::queueJob([this, veh] {
                    m_chaff_busy = true;
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(veh, TRUE);
                    STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_sm_counter");
                    while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_sm_counter"))
                        Script::current()->yield();
                    GRAPHICS::USE_PARTICLE_FX_ASSET("scr_sm_counter");
                    GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(
                        "scr_sm_counter_chaff", pos.x, pos.y, pos.z,
                        0.f, 180.f, 0.f, 5.f, true, true, true, false);
                    VEHICLE::SET_VEHICLE_CAN_BE_TARGETTED(veh, FALSE);
                    Script::current()->yield(3000);
                    VEHICLE::SET_VEHICLE_CAN_BE_TARGETTED(veh, TRUE);
                    m_chaff_busy = false;
                });
            }

            void deployFlares(int veh)
            {
                const int count = m_count->value;
                const Hash flare = MISC::GET_HASH_KEY("WEAPON_FLAREGUN");
                const Hash shell = MISC::GET_HASH_KEY("w_pi_flaregun_shell");
                FiberPool::queueJob([this, veh, count, flare, shell] {
                    STREAMING::REQUEST_MODEL(shell);
                    while (!STREAMING::HAS_MODEL_LOADED(shell))
                        Script::current()->yield();
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "flares_released", veh,
                        "DLC_SM_Countermeasures_Sounds", TRUE, 0);
                    for (int i = 0; i < count; i++)
                    {
                        Vector3 pat = m_pattern->getVec();
                        Vector3 pos = ENTITY::GET_ENTITY_COORDS(veh, TRUE);
                        Vector3 off1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -pat.x, pat.y, pat.z);
                        Vector3 off2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh,  pat.x, pat.y, pat.z);
                        MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
                            pos.x, pos.y, pos.z, off1.x, off1.y, off1.z,
                            1, TRUE, flare, ped, TRUE, TRUE, 1.f);
                        Script::current()->yield(25);
                        MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
                            pos.x, pos.y, pos.z, off2.x, off2.y, off2.z,
                            1, TRUE, flare, ped, TRUE, TRUE, 1.f);
                        Script::current()->yield(200);
                    }
                });
            }

            void deployRelevant(CM type, int veh)
            {
                if (type != FLARE) deployChaff(veh);
                if (type != CHAFF) deployFlares(veh);
            }

            bool canDeploy(int veh) { return shouldDeploy(veh); }

            void onClick(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
                    if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, FALSE) != ped) return;
                    if (!shouldDeploy(veh)) return;
                    deployRelevant(m_cm, veh);
                });
            }
        };

        class CommandAutoDeployCm : public CommandSlider
        {
            static constexpr const char* kNames[] = {"Disabled", "Flares", "Chaff", "Both"};

            time_t m_last_flare = 0;
            time_t m_last_periodic = 0;
            bool m_ticking = false;
            CommandFireCm* const m_fire;
            CommandToggle* const m_periodic;
            CommandSlider* const m_periodic_delay;

        public:
            explicit CommandAutoDeployCm(CommandList* parent, CommandFireCm* fire,
                CommandToggle* periodic, CommandSlider* periodic_delay)
                : CommandSlider(parent, LIT("Auto-Deploy"), CMDNAMES("autodeploy"), NOLABEL, 0, 3, 0)
                , m_fire(fire), m_periodic(periodic), m_periodic_delay(periodic_delay) {}

            ~CommandAutoDeployCm() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override { return kNames[value]; }

            void onChange(Click& click, int prev_value) override
            {
                if (click.isAuto()) return;
                const bool was_active = prev_value != 0;
                const bool is_active  = value != 0;
                if (is_active && !was_active)
                {
                    CommandTickDispatch::AddCommand(this);
                    m_ticking = true;
                }
                else if (!is_active && was_active)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            }

            void onTick() override
            {
                if (value == 0) return;

                int ped = PLAYER::GET_PLAYER_PED(-1);
                int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
                if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, FALSE) != ped) return;
                if (!m_fire->canDeploy(veh)) return;

                const bool do_flares = value == 1 || value == 3;
                const bool do_chaff  = value == 2 || value == 3;

                if (m_periodic->m_on
                    && GET_MILLIS_SINCE(m_last_periodic) >= static_cast<time_t>(m_periodic_delay->value * 1000))
                {
                    m_last_periodic = get_current_time_millis();
                    if (do_chaff)  m_fire->deployChaff(veh);
                    if (do_flares) m_fire->deployFlares(veh);
                    return;
                }

                if (do_chaff && VEHICLE::GET_VEHICLE_HOMING_LOCKEDONTO_STATE(veh) != 0)
                    m_fire->deployChaff(veh);

                if (do_flares
                    && VEHICLE::GET_VEHICLE_HOMING_LOCKEDONTO_STATE(veh) == 2
                    && GET_MILLIS_SINCE(m_last_flare) >= 2000)
                {
                    m_fire->deployFlares(veh);
                    m_last_flare = get_current_time_millis();
                }
            }
        };

        class CommandCmDivider : public CommandPhysical
        {
        public:
            explicit CommandCmDivider(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Settings"), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };
    }

    CommandListCountermeasures::CommandListCountermeasures(CommandList* parent)
        : CommandList(parent, LIT("Countermeasures"), CMDNAMES("countermeasures"))
    {
        auto count          = makeChild<CommandSlider>(LIT("Count"), CMDNAMES("cmcount"), NOLABEL, 1, 10, 2);
        auto onlyair        = makeChild<CommandToggle>(LIT("Only In Aircraft"), CMDNAMES("onlyaircraft"),
                                  LIT("Only deploys when in an aircraft or the Oppressor Mk II."));
        auto pattern        = makeChild<CommandCmPattern>();
        auto periodic       = makeChild<CommandToggle>(LIT("Periodic Deployment"), CMDNAMES("periodiccm"));
        auto periodic_delay = makeChild<CommandSlider>(LIT("Periodic Deployment: Delay"),
                                  CMDNAMES("periodiccmdelay"), NOLABEL, 1, 10, 2);

        auto fire_chaff = makeChild<CommandFireCm>(CommandFireCm::CHAFF, LIT("Deploy Chaff"),
                              CMDNAMES("deploychaff"), count.get(), onlyair.get(), pattern.get());
        auto fire_flare = makeChild<CommandFireCm>(CommandFireCm::FLARE, LIT("Deploy Flares"),
                              CMDNAMES("deployflares"), count.get(), onlyair.get(), pattern.get());
        auto fire_both  = makeChild<CommandFireCm>(CommandFireCm::BOTH,  LIT("Deploy Flares & Chaff"),
                              CMDNAMES("deployboth"), count.get(), onlyair.get(), pattern.get());

        auto auto_deploy = makeChild<CommandAutoDeployCm>(fire_flare.get(), periodic.get(), periodic_delay.get());

        children.emplace_back(std::move(auto_deploy));
        children.emplace_back(std::move(fire_chaff));
        children.emplace_back(std::move(fire_flare));
        children.emplace_back(std::move(fire_both));
        createChild<CommandCmDivider>();
        children.emplace_back(std::move(count));
        children.emplace_back(std::move(onlyair));
        children.emplace_back(std::move(pattern));
        children.emplace_back(std::move(periodic));
        children.emplace_back(std::move(periodic_delay));
    }
}
