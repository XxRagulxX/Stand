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
	class CommandWalkUnderwater : public CommandToggle
	{
	public:
		explicit CommandWalkUnderwater(CommandList* const parent)
			: CommandToggle(parent, LIT("Walk Underwater"), CMDNAMES("walkunderwater"), LIT("Walk on the seabed instead of swimming."))
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
			ped.SetConfigFlag(PedConfigFlag::IsSwimming, false);
			ped.SetConfigFlag(PedConfigFlag::SwimmingTasksRunning, false);
		}

		~CommandWalkUnderwater() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
