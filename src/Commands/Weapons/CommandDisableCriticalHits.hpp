#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandDisableCriticalHits : public CommandToggle
	{
	public:
		explicit CommandDisableCriticalHits(CommandList* const parent)
			: CommandToggle(parent, LIT("Disable Critical Hits"), CMDNAMES("disablecriticalhits"), LIT("Prevents enemies from dealing critical hits to you."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_SUFFERS_CRITICAL_HITS(ped.GetHandle(), TRUE);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_SUFFERS_CRITICAL_HITS(ped.GetHandle(), FALSE);
		}

		~CommandDisableCriticalHits() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
