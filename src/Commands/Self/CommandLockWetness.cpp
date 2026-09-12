#include "Commands/Self/CommandLockWetness.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandLockWetness::onEnable(Click& click)
	{
		CommandTickDispatch::AddCommand(this);
	}

	void CommandLockWetness::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	void CommandLockWetness::onTick()
	{
		const int ped = Self::GetPed().GetHandle();
		if (m_wetness->value == 0)
		{
			PED::CLEAR_PED_WETNESS(ped);
		}
		else
		{
			PED::SET_PED_WETNESS_HEIGHT(ped, (((float)m_wetness->value * 0.2f) - 110.0f) / 100.0f);
			PED::SET_PED_WETNESS_ENABLED_THIS_FRAME(ped);
		}
	}

	void CommandLockWetness::applyDefaultState()
	{
		if (m_on)
		{
			m_on = false;
			CommandTickDispatch::RemoveCommand(this);
		}
	}
}
