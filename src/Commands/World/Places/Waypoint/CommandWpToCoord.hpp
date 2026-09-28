#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandWpToCoord : public CommandPhysical
    {
    public:
        float m_x, m_y;

        CommandWpToCoord(CommandList* parent, Label&& label, float x, float y)
            : CommandPhysical(COMMAND_ACTION, parent, std::move(label), CMDNAMES()),
              m_x(x), m_y(y)
        {
        }

        void onClick(Click& click) override
        {
            click.ensureScriptThread([this] {
                HUD::SET_NEW_WAYPOINT(m_x, m_y);
            });
        }
    };
}
