#include "Commands/Self/CommandNoBlood.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandNoBlood::onEnable(Click& click)
	{
		CommandTickDispatch::AddCommand(this);
	}

	void CommandNoBlood::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	void CommandNoBlood::onTick()
	{
		PED::CLEAR_PED_BLOOD_DAMAGE(Self::GetPed().GetHandle());
	}

	void CommandNoBlood::applyDefaultState()
	{
		if (m_on)
		{
			m_on = false;
			CommandTickDispatch::RemoveCommand(this);
		}
	}
}
