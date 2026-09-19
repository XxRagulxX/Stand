#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Ped/PedConfigFlag.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandSwimInAir : public CommandToggle
	{
	public:
		explicit CommandSwimInAir(CommandList* const parent)
			: CommandToggle(parent, LIT("Swim In Air"), CMDNAMES("swiminair"), LIT("Swim while in mid-air."))
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
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;
			ped.SetConfigFlag(PedConfigFlag::IsSwimming, true);
		}

		~CommandSwimInAir() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
