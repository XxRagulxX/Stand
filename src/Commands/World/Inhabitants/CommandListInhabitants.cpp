#include "Commands/World/Inhabitants/CommandListInhabitants.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandToggleField.hpp"
#include "Commands/World/Inhabitants/CommandClearArea.hpp"
#include "Commands/World/Inhabitants/CommandDisablePeds.hpp"
#include "Commands/World/Inhabitants/CommandExistencePunishment.hpp"
#include "Commands/World/Inhabitants/CommandNpcAimPunishment.hpp"
#include "Commands/World/Inhabitants/CommandNpcHostilityPunishment.hpp"
#include "Commands/World/Inhabitants/CommandNpcNeedsToAimAtUser.hpp"
#include "Commands/World/Inhabitants/CommandNpcProximityPunishment.hpp"
#include "Commands/World/Inhabitants/CommandNpcBoneEsp.hpp"
#include "Commands/World/Inhabitants/CommandNpcEspColour.hpp"
#include "Commands/World/Inhabitants/CommandPlayerEspColour.hpp"
#include "Commands/World/Inhabitants/CommandPlayerEspTagColours.hpp"
#include "Commands/World/Inhabitants/CommandPlayerEspToggles.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/World/Inhabitants/CommandPlayerAimPunishment.hpp"
#include "Commands/World/Inhabitants/CommandPlayerNeedsToAimAtUser.hpp"
#include "Commands/World/Inhabitants/CommandPunishableProximity.hpp"
#include "Commands/World/Inhabitants/CommandTrafficColour.hpp"
#include "Commands/World/Inhabitants/CommandTrafficDisable.hpp"
#include "Commands/World/Inhabitants/CommandTrafficLod.hpp"
#include "Commands/World/Inhabitants/CommandTrafficModelFlag.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/Punishments.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandSectionHeader : public CommandPhysical
    {
    public:
        CommandSectionHeader(CommandList* parent, const char* label)
            : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
    };

    class CommandDeleteRopes : public CommandPhysical
    {
    public:
        explicit CommandDeleteRopes(CommandList* parent)
            : CommandPhysical(COMMAND_ACTION, parent, LIT("Delete All Ropes"), CMDNAMES("deleteropes"))
        {}

        void onClick(Click& click) final
        {
            click.ensureScriptThread([] {
                for (int i = 0; i < 100; ++i) {
                    int tmp = i;
                    PHYSICS::DELETE_ROPE(&tmp);
                    PHYSICS::DELETE_CHILD_ROPE(i);
                }
            });
        }
    };

    class CommandMaxDistSlider : public CommandSlider
    {
        int* m_field;
    public:
        CommandMaxDistSlider(CommandList* parent, std::vector<CommandName> cmdnames, int* field)
            : CommandSlider(parent, LIT("Max Distance"), std::move(cmdnames), NOLABEL, 1, 100000, 1000, 50)
            , m_field(field)
        {}
        void onChange(Click& click, int) override { *m_field = value; }
    };

    class CommandIntFieldSlider : public CommandSlider
    {
        int* m_field;
    public:
        CommandIntFieldSlider(CommandList* parent, Label name, std::vector<CommandName> cmdnames,
                              int* field, int min_v, int max_v, int def_v, unsigned int step = 1)
            : CommandSlider(parent, std::move(name), std::move(cmdnames), NOLABEL, min_v, max_v, def_v, step)
            , m_field(field)
        {}
        void onChange(Click& click, int) override { *m_field = value; }
    };

    CommandListInhabitants::CommandListInhabitants(CommandList* parent)
        : CommandList(parent, LIT("Inhabitants"), CMDNAMES("inhabitants"))
    {
        createChild<CommandClearArea>();

        // Traffic
        {
            auto* traffic = createChild<CommandList>(LIT("Traffic"));
            traffic->createChild<CommandTrafficColour>();
            traffic->createChild<CommandTrafficDisable>();
            traffic->createChild<CommandTrafficLod>();
            traffic->createChild<CommandTrafficModelFlag>();
        }

        // Pedestrians
        {
            auto* peds = createChild<CommandList>(LIT("Pedestrians"));
            peds->createChild<CommandDisablePeds>();
        }

        // NPC Existence Punishments
        {
            auto* existence_punishments = createChild<CommandList>(LIT("NPC Existence Punishments"), CMDNAMES("existencepunishments"));
            for (const Punishment* const p : Punishments::all) {
                if (p->isApplicable(PUNISHMENTFOR_NPC)) {
                    existence_punishments->createChild<CommandExistencePunishment>(*p);
                }
            }
        }

        // NPC Proximity Punishments
        {
            auto* proximity_punishments = createChild<CommandList>(LIT("NPC Proximity Punishments"), CMDNAMES("proximitypunishments"));
            proximity_punishments->createChild<CommandPunishableProximity>();
            for (const Punishment* const p : Punishments::all) {
                if (p->isApplicable(PUNISHMENTFOR_NPC | PUNISHMENTFOR_NPCHOSTILITY)
                    && p->mask != PUNISHMENT_REVIVE
                    && p->mask != PUNISHMENT_WEAKEN) {
                    proximity_punishments->createChild<CommandNpcProximityPunishment>(*p);
                }
            }
        }

        // NPC Hostility Punishments
        {
            auto* list = createChild<CommandList>(LIT("NPC Hostility Punishments"), CMDNAMES("hostilitypunishments"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_everyone, LIT("Include Everyone"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_everyone_in_missions, LIT("Include Everyone, In Missions"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_friends, LIT("Include Friends"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_passengers, LIT("Include Passengers"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_crew, LIT("Include Crew Members"));
            list->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_hostility_include_org, LIT("Include Organisation Members"));
            list->createChild<CommandSectionHeader>("NPC Hostility Punishments");
            for (const Punishment* const p : Punishments::all) {
                if (p->isApplicable(PUNISHMENTFOR_NPCHOSTILITY)) {
                    list->createChild<CommandNpcHostilityPunishment>(*p);
                }
            }
        }

        // NPC Aim Punishments
        {
            auto* npc_aim_punishments = createChild<CommandList>(LIT("NPC Aim Punishments"), CMDNAMES("aimpunishments"),
                LIT("Punishments applied to NPCs that are aiming at you."));
            npc_aim_punishments->createChild<CommandNpcNeedsToAimAtUser>();
            npc_aim_punishments->createChild<CommandSectionHeader>("NPC Aim Punishments");
            for (const Punishment* const p : Punishments::all) {
                if (p->isApplicable(PUNISHMENTFOR_NPC | PUNISHMENTFOR_AIMINGAUSER)) {
                    npc_aim_punishments->createChild<CommandNpcAimPunishment>(*p);
                }
            }
        }

        // Player Aim Punishments
        {
            auto* player_aim_punishments = createChild<CommandList>(LIT("Player Aim Punishments"), CMDNAMES("playeraimpunishments"),
                LIT("Punishments applied to players that are aiming at you."));
            player_aim_punishments->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_aim_exclude_friends, LIT("Exclude Friends"));
            player_aim_punishments->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_aim_exclude_crew, LIT("Exclude Crew Members"));
            player_aim_punishments->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_aim_exclude_stand_users, LIT("Exclude Stand Users"));
            player_aim_punishments->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_aim_exclude_org, LIT("Exclude Organisation Members"));
            player_aim_punishments->createChild<CommandPlayerNeedsToAimAtUser>();
            player_aim_punishments->createChild<CommandSectionHeader>("Player Aim Punishments");
            for (const Punishment* const p : Punishments::all) {
                if (p->isApplicable(PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_AIMINGAUSER)) {
                    player_aim_punishments->createChild<CommandPlayerAimPunishment>(*p);
                }
            }
        }

        // NPC ESP
        {
            auto* npc_esp = createChild<CommandList>(LIT("NPC ESP"), CMDNAMES("npcesplist"));
            npc_esp->createChild<CommandNpcEspColour>();
            npc_esp->createChild<CommandToggleField>(&AllEntitiesEveryTick::npc_bone_esp_exclude_dead, LIT("Exclude Dead"));
            npc_esp->createChild<CommandNpcBoneEsp>();
        }

        // Player ESP
        {
            auto* player_esp = createChild<CommandList>(LIT("Player ESP"), CMDNAMES("playeresplist"));
            player_esp->createChild<CommandPlayerEspBone>();
            {
                auto* name_esp = player_esp->createChild<CommandList>(LIT("Name ESP"));
                name_esp->createChild<CommandMaxDistSlider>(CMDNAMES("esprange"), &AllEntitiesEveryTick::player_esp_name_max_dist);
                name_esp->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_esp_name_show_tags, LIT("Show Tags"));
                name_esp->createChild<CommandIntFieldSlider>(LIT("Min Text Scale"), CMDNAMES("esptextmin"), &AllEntitiesEveryTick::player_esp_name_min_scale, 1, 10000, 50);
                name_esp->createChild<CommandIntFieldSlider>(LIT("Max Text Scale"), CMDNAMES("esptextmax"), &AllEntitiesEveryTick::player_esp_name_max_scale, 1, 10000, 100);
                name_esp->createChild<CommandToggleField>(&AllEntitiesEveryTick::player_esp_name_invert_scale, LIT("Invert Text Scaling"));
                name_esp->createChild<CommandPlayerEspName>();
            }
            {
                auto* box_esp = player_esp->createChild<CommandList>(LIT("Box ESP"));
                box_esp->createChild<CommandMaxDistSlider>(CMDNAMES("boxesprange"), &AllEntitiesEveryTick::player_esp_box_max_dist);
                box_esp->createChild<CommandPlayerEspBox>();
            }
            {
                auto* line_esp = player_esp->createChild<CommandList>(LIT("Line ESP"));
                line_esp->createChild<CommandMaxDistSlider>(CMDNAMES("lineesprange"), &AllEntitiesEveryTick::player_esp_line_max_dist);
                line_esp->createChild<CommandPlayerEspLine>();
            }
            player_esp->createChild<CommandSectionHeader>("Colours");
            player_esp->createChild<CommandPlayerEspColour>();
            player_esp->createChild<CommandPlayerEspTagColours>();
        }

        createChild<CommandDeleteRopes>();
    }
}
