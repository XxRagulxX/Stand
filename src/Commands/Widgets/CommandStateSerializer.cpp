#include "Commands/Widgets/CommandStateSerializer.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

#include <algorithm>

namespace Stand
{
	namespace
	{
		// CommandIssuable::command_names can hold several aliases
		// (CMDNAMES(...)) purely for lookup (see CommandRegistry.hpp) -
		// persistence needs exactly one stable key per command, so this
		// always uses the first one.
		const CommandName* PrimaryName(CommandPhysical* command)
		{
			return command->command_names.empty() ? nullptr : &command->command_names.front();
		}
	}

	CommandStateSerializer::CommandStateSerializer() :
	    IStateSerializer("stand_commands")
	{
	}

	void CommandStateSerializer::AddCommandImpl(CommandPhysical* command)
	{
		if (command)
			m_Commands.push_back(command);
	}

	void CommandStateSerializer::RemoveCommandImpl(CommandPhysical* command)
	{
		std::erase(m_Commands, command);
	}

	void CommandStateSerializer::SaveStateImpl(nlohmann::json& state)
	{
		for (auto* command : m_Commands)
		{
			if (!command->supportsSavedState())
				continue;

			const auto* name = PrimaryName(command);
			if (!name)
				continue;

			state[*name] = command->getState();
		}
	}

	void CommandStateSerializer::LoadStateImpl(nlohmann::json& state)
	{
		for (auto* command : m_Commands)
		{
			const auto* name = PrimaryName(command);
			if (!name)
				continue;

			// Applied through queueJob(), not called inline - this runs
			// during Settings::InitializeImpl(), long before any script
			// thread is guaranteed ready, and setState()/
			// applyDefaultState() both end up touching game natives
			// through a real command's onEnable()/onChange() (e.g.
			// CommandGod's SetInvincible()) - same "queue it, don't call
			// it inline" convention as every other click dispatch this
			// session (GridItemStandCommand.cpp's own ToggleClicked()/
			// SliderStep()/ButtonClicked()).
			if (command->supportsSavedState() && state.contains(*name) && state[*name].is_string())
			{
				auto saved = state[*name].get<std::string>();
				command->queueJob([command, saved = std::move(saved)] {
					Click click(CLICK_BULK, TC_SCRIPT_YIELDABLE);
					command->setState(click, saved);
				});
			}
			else
			{
				command->queueJob([command] {
					command->applyDefaultState();
				});
			}
		}
	}
}
