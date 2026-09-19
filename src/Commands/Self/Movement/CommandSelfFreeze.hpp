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
	class CommandSelfFreeze : public CommandToggle
	{
	public:
		explicit CommandSelfFreeze(CommandList* const parent)
			: CommandToggle(parent, LIT("Freeze"), CMDNAMES("selffreeze"), LIT("Freezes you in place."))
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
				ENTITY::FREEZE_ENTITY_POSITION(ped.GetHandle(), false);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				ENTITY::FREEZE_ENTITY_POSITION(ped.GetHandle(), true);
		}

		~CommandSelfFreeze() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
