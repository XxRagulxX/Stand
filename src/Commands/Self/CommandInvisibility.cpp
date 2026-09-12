#include "Commands/Self/CommandInvisibility.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	CommandInvisibility::CommandInvisibility(CommandList* parent)
		: CommandSlider(parent, LIT("Invisibility"), CMDNAMES("invisibility"), LIT("When in a vehicle, other players will still be able to see you unless you also make your vehicle invisible."), 0, 2, 0, 1, 0)
	{
		CommandTickDispatch::AddCommand(this);
	}

	CommandInvisibility::~CommandInvisibility()
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	std::string CommandInvisibility::getValueText() const
	{
		switch (value)
		{
		case 0: return "Disabled";
		case 1: return "Locally Visible";
		case 2: return "Enabled";
		default: return CommandSlider::getValueText();
		}
	}

	void CommandInvisibility::onChange(Click& click, int prev_value)
	{
	}

	void CommandInvisibility::onTick()
	{
		const int ped = Self::GetPed().GetHandle();
		if (value == 0)
		{
			if (m_applied != 0)
			{
				m_applied = 0;
				ENTITY::SET_ENTITY_VISIBLE(ped, true, false);
			}
			return;
		}
		m_applied = value;
		ENTITY::SET_ENTITY_VISIBLE(ped, false, false);
		if (value == 1 && *Pointers.IsSessionStarted)
			NETWORK::SET_ENTITY_LOCALLY_VISIBLE(ped);
	}

	void CommandInvisibility::applyDefaultState()
	{
		m_applied = -1;
		value = 0;
	}
}
