#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggleField.hpp"
#include "Commands/World/Inhabitants/CommandEspColourPicker.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandPlayerEspTagColours : public CommandList
    {
    public:
        explicit CommandPlayerEspTagColours(CommandList* parent)
            : CommandList(parent, LIT("Tag Colours"), CMDNAMES("tagcolours"))
        {
            using AEET = AllEntitiesEveryTick;
            auto addTag = [&](const char* label, AEET::PlayerEspTagEntry& entry, const char* cmdname) {
                auto* tag = createChild<CommandList>(Label(label, Label::TagLiteral{}));
                tag->createChild<CommandEspColourPicker>(LIT("Colour"), std::vector<CommandName>{CommandName(cmdname)}, &entry.r, &entry.g, &entry.b);
                tag->createChild<CommandToggleField>(&entry.use, LIT("Use Colour"));
            };

            addTag("Friend",                AEET::player_esp_tag_friend,          "espcolourfrnd");
            addTag("Organisation Member",   AEET::player_esp_tag_org,             "espcolourorg");
            addTag("Modder",                AEET::player_esp_tag_modder,          "espcolourmod");
            addTag("Likely Modder",         AEET::player_esp_tag_likely_modder,   "espcolourlmod");
            addTag("Invulnerable",          AEET::player_esp_tag_invulnerable,    "espcolourinvul");
            addTag("Indestructible Vehicle",AEET::player_esp_tag_veh_god,         "espcolourvehgod");
            addTag("Off The Radar",         AEET::player_esp_tag_otr,             "espcolourotr");
            addTag("Invisible",             AEET::player_esp_tag_invisible,       "espcolourinvis");
            addTag("Crew Member",           AEET::player_esp_tag_crew,            "espcolourcrew");
            addTag("Dead",                  AEET::player_esp_tag_dead,            "espcolourdead");
            addTag("RC Vehicle",            AEET::player_esp_tag_rc,              "espcolourrc");
        }
    };
}
