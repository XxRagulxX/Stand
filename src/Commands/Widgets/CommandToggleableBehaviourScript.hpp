#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/tbToggleableBehaviourScript.hpp"

namespace Stand
{
	class CommandToggleableBehaviourScript : public CommandToggle
	{
	private:
		tbToggleableBehaviourScript* const toggleable_behaviour;

	public:
		explicit CommandToggleableBehaviourScript(CommandList* const parent, tbToggleableBehaviourScript* const toggleable_behaviour, Label&& menu_name, std::vector<CommandName>&& command_names)
			: CommandToggle(parent, std::move(menu_name), std::move(command_names)), toggleable_behaviour(toggleable_behaviour)
		{
		}

		void onEnable(Click& click) final
		{
			toggleable_behaviour->enable();
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) final
		{
			CommandTickDispatch::RemoveCommand(this);
			toggleable_behaviour->disable();
		}

		void onTick() override
		{
			if (toggleable_behaviour->isEnabled())
			{
				toggleable_behaviour->runTick();
			}
		}

		~CommandToggleableBehaviourScript() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
