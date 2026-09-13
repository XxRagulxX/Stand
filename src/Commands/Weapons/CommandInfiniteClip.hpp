#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandInfiniteClip : public CommandToggle
	{
	public:
		explicit CommandInfiniteClip(CommandList* const parent)
			: CommandToggle(parent, LIT("Infinite Clip"), CMDNAMES("infiniteclip"), LIT("Never need to reload."))
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
				ped.SetInfiniteClip(false);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetInfiniteClip(true);
		}

		~CommandInfiniteClip() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
