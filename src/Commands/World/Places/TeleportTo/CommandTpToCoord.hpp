#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTpToCoord : public CommandPhysical
    {
    public:
        float m_x, m_y, m_z;

        CommandTpToCoord(CommandList* parent, Label&& label, float x, float y, float z)
            : CommandPhysical(COMMAND_ACTION, parent, std::move(label), CMDNAMES()),
              m_x(x), m_y(y), m_z(z)
        {
        }

        void onClick(Click& click) override
        {
            click.ensureScriptThread([this] {
                TpUtil::DoTeleport(m_x, m_y, m_z);
            });
        }
    };
}
