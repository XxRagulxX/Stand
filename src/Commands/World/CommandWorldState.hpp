#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Game/WorldState.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandWorldState : public CommandList
    {
    private:
        static void islandToggle(bool t)
        {
            STREAMING::LOAD_GLOBAL_WATER_FILE(t ? 1 : 0);
            STREAMING::SET_ISLAND_ENABLED("HeistIsland", t);
            TASK::SET_SCENARIO_GROUP_ENABLED("Heist_Island_Peds", t);
            HUD::SET_USE_ISLAND_MAP(t);
            AUDIO::SET_AUDIO_FLAG("PlayerOnDLCHeist4Island", t);
            AUDIO::SET_AMBIENT_ZONE_LIST_STATE_PERSISTENT("AZL_DLC_Hei4_Island_Zones", t, true);
            AUDIO::SET_AMBIENT_ZONE_LIST_STATE_PERSISTENT("AZL_DLC_Hei4_Island_Disabled_Zones", !t, true);
            if (!t)
                AUDIO::RELEASE_NAMED_SCRIPT_AUDIO_BANK("DLC_HEI4/DLCHEI4_GENERIC_01");
        }

        class CommandWorldStateStoryMode : public CommandPhysical
        {
        public:
            explicit CommandWorldStateStoryMode(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Story Mode"), CMDNAMES("storymode", "singleplayer", "sp"), NOLABEL)
            {
            }

            void onClick(Click& click) override
            {
                ensureScriptThread(click, [] {
                    islandToggle(false);
                    WorldState::setOnline(FALSE);
                });
            }
        };

        class CommandWorldStateOnline : public CommandPhysical
        {
        public:
            explicit CommandWorldStateOnline(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Online"), CMDNAMES("online", "mp"), NOLABEL)
            {
            }

            void onClick(Click& click) override
            {
                ensureScriptThread(click, [] {
                    islandToggle(false);
                    WorldState::setOnline(TRUE);
                });
            }
        };

        class CommandWorldStateCayo : public CommandPhysical
        {
        public:
            explicit CommandWorldStateCayo(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Cayo Perico"), CMDNAMES("island", "cayoperico"), NOLABEL)
            {
            }

            void onClick(Click& click) override
            {
                ensureScriptThread(click, [] {
                    WorldState::setOnline(TRUE);
                    islandToggle(true);
                });
            }
        };

    public:
        explicit CommandWorldState(CommandList* parent)
            : CommandList(parent, LIT("Set World State"), CMDNAMES("worldstate"), LIT("Changing the world state might make the game unresponsive shortly."))
        {
            createChild<CommandWorldStateStoryMode>();
            createChild<CommandWorldStateOnline>();
            createChild<CommandWorldStateCayo>();
        }
    };
}
