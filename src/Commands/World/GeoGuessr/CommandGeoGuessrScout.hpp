#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessr.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandGeoGuessrScout : public CommandToggle
    {
    public:
        explicit CommandGeoGuessrScout(CommandList* parent)
            : CommandToggle(parent, LIT("View Target"), {}, NOLABEL, true,
                (CMDFLAGS_TOGGLE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS) | CMDFLAG_CONCEALED)
        {
        }

        void onEnable(Click& click) override
        {
            ensureYieldableScriptThread(click, [this]
            {
                if (parent->as<CommandGeoGuessr>()->guessed_at == 0)
                    parent->as<CommandGeoGuessr>()->startScouting();
            });
        }

        void onDisable(Click& click) override
        {
            ensureScriptThread(click, [this]
            {
                parent->as<CommandGeoGuessr>()->stopScouting();
            });
        }
    };
}
