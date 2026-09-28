#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    class CommandNorthYanktonMap : public CommandToggle
    {
    public:
        explicit CommandNorthYanktonMap(CommandList* parent)
            : CommandToggle(parent, LIT("North Yankton Map & Radar"), CMDNAMES("yankmap", "northyanktonmap"))
        {
        }

        void onChange(Click& click) final
        {
            click.ensureScriptThread([this] {
                HUD::SET_MINIMAP_IN_PROLOGUE(m_on ? TRUE : FALSE);
            });
        }
    };
}
