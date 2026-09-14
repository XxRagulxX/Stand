#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandGunFreedom : public CommandToggle
	{
	public:
		explicit CommandGunFreedom(CommandList* const parent)
			: CommandToggle(parent, LIT("Total Freedom"),
			                CMDNAMES("gunfreedom"),
			                LIT("Allows you to use weapons in interiors and in passive mode."))
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
			MISC::SET_FIRE_AMMO_THIS_FRAME(Stand::Self::GetPlayer().GetId());
		}

		~CommandGunFreedom() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
