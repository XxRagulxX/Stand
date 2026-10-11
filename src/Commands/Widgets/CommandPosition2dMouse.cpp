#include "Commands/Widgets/CommandPosition2dMouse.hpp"

#include "Rendering/StandPort/CommandPosition2d.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandPosition2dMouse::CommandPosition2dMouse(CommandList* parent, const CommandName& command_name)
		: CommandToggle(parent, LOC("MOVWTHMSE"), { std::move(CommandName(command_name).append(CMDNAME("mousemove"))) }, NOLABEL, false, CMDFLAGS_TOGGLE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
	{
	}

	void CommandPosition2dMouse::onEnable(Click& click)
	{
	}

	void CommandPosition2dMouse::updatePos(DirectX::SimpleMath::Vector2&& cursor_pos) const
	{
	}

	void CommandPosition2dMouse::onDisable(Click& click)
	{
	}
}
