#include "Commands/Self/CommandPreserveWalkStyle.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandPreserveWalkStyle::onEnable(Click& click)
	{
		CommandTickDispatch::AddCommand(this);
	}

	void CommandPreserveWalkStyle::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	void CommandPreserveWalkStyle::onTick()
	{
		PED::SET_PED_RESET_FLAG(Self::GetPed().GetHandle(), 200, true);
	}

	void CommandPreserveWalkStyle::applyDefaultState()
	{
		if (m_on)
		{
			m_on = false;
			CommandTickDispatch::RemoveCommand(this);
		}
	}
}
