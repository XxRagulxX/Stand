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
	class CommandTotalFreedom : public CommandToggle
	{
	public:
		explicit CommandTotalFreedom(CommandList* const parent)
			: CommandToggle(parent, LIT("Total Freedom"), CMDNAMES("movefreedom"), LIT("Allows you to sprint, sneak, and jump in interiors."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::RESET_PED_MOVEMENT_CLIPSET(ped.GetHandle(), 0.0f);
		}

		~CommandTotalFreedom() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
