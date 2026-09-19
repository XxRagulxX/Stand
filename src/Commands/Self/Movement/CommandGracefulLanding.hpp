#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandGracefulLanding : public CommandToggle
	{
	public:
		explicit CommandGracefulLanding(CommandList* const parent)
			: CommandToggle(parent, LIT("Graceful Landing"), CMDNAMES("gracefullanding"), NOLABEL)
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
			{
				PED::SET_DISABLE_HIGH_FALL_DEATH(ped.GetHandle(), false);
				PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), true);
			}
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				PED::SET_DISABLE_HIGH_FALL_DEATH(ped.GetHandle(), true);
				PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), false);
			}
		}

		~CommandGracefulLanding() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
