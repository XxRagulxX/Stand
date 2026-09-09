#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Self/CommandWanted.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandFreezeWanted : public CommandToggle
	{
		CommandWanted* const m_wanted;

	public:
		explicit CommandFreezeWanted(CommandList* const parent, CommandWanted* wanted)
			: CommandToggle(parent, LIT("Lock Wanted Level"), { CMDNAME("freezewanted") }, NOLABEL)
			, m_wanted(wanted)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			PLAYER::SET_MAX_WANTED_LEVEL(6);
		}

		void onTick() override
		{
			const int level = m_wanted->value;
			if (Stand::Self::GetPlayer().GetWantedLevel() != level)
				Stand::Self::GetPlayer().SetWantedLevel(level);
			PLAYER::SET_MAX_WANTED_LEVEL(level);
		}

		~CommandFreezeWanted() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
