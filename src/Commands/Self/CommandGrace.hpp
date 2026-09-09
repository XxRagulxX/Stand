#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandGrace : public CommandToggle
	{
	public:
		explicit CommandGrace(CommandList* const parent)
			: CommandToggle(parent, LIT("Gracefulness"), { CMDNAME("grace"), CMDNAME("gracefulness"), CMDNAME("noragdoll") }, LIT("Makes your character unable to fall over."))
		{
		}

		void onEnable(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetRagdoll(false);
			Stand::Self::GetPlayer().SetFallDistanceOverride(99999.0f);
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				ped.SetRagdoll(true);
			Stand::Self::GetPlayer().SetFallDistanceOverride(0.0f);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetRagdoll(false);
			Stand::Self::GetPlayer().SetFallDistanceOverride(99999.0f);
		}

		~CommandGrace() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
