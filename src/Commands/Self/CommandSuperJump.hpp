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
	class CommandSuperJump : public CommandToggle
	{
	public:
		explicit CommandSuperJump(CommandList* const parent)
			: CommandToggle(parent, LIT("Super Jump"), CMDNAMES("superjump"), NOLABEL)
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
			MISC::SET_SUPER_JUMP_THIS_FRAME(Stand::Self::GetPlayer().GetId());
		}

		~CommandSuperJump() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
