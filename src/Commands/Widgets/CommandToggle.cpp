#include "CommandToggle.hpp"

#include "Menu/Click.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandToggle::CommandToggle(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, const bool default_on, const commandflags_t flags, const CommandPerm perm, const std::vector<Hotkey>& default_hotkeys)
		: CommandToggleNoCorrelation(parent, std::move(menu_name), std::move(command_names), std::move(help_text), default_on, flags, perm, default_hotkeys, COMMAND_TOGGLE)
	{
	}

	CommandToggle::CommandToggle(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, ToggleCorrelation::Type correlation_type)
		: CommandToggleNoCorrelation(parent, std::move(menu_name), std::move(command_names), std::move(help_text), false, CMDFLAGS_TOGGLE, COMMANDPERM_USERONLY, {}, COMMAND_TOGGLE),
		  correlation(correlation_type), default_correlation(correlation_type)
	{
		g_gui.commands_with_correlation.emplace_back(this);
	}

	void CommandToggle::preDelete()
	{
		CommandToggleNoCorrelation::preDelete();
		if (correlation.isActive())
		{
			auto i = std::find(g_gui.commands_with_correlation.begin(), g_gui.commands_with_correlation.end(), this);
			if (i != g_gui.commands_with_correlation.end())
				g_gui.commands_with_correlation.erase(i);
		}
	}

	void CommandToggle::onClick(Click& click)
	{
		onClickRemoveCorrelation(click);
		CommandToggleNoCorrelation::onClick(click);
	}

	void CommandToggle::onClickRemoveCorrelation(Click& click)
	{
		if (correlation.isActive() && !click.isAuto())
		{
			auto click_ = click.deriveAuto();
			removeCorrelation(click_);
		}
	}

	std::string CommandToggle::getState() const
	{
		if (correlation.isActive())
			return correlation.getState();
		return CommandToggleNoCorrelation::getState();
	}

	std::string CommandToggle::getDefaultState() const
	{
		if (default_correlation.isActive())
			return default_correlation.getState();
		return CommandToggleNoCorrelation::getDefaultState();
	}

	void CommandToggle::setState(Click& click, const std::string& _state)
	{
		auto space_pos = _state.find(' ');
		if (space_pos != std::string::npos)
		{
			std::string state(_state);
			bool invert = (state.substr(0, space_pos) == "Off");
			state.erase(0, space_pos + 7); // " while "
			for (ToggleCorrelation_t i = ToggleCorrelation::NONE + 1; i < ToggleCorrelation::_NUM_TOGGLE_CORRELATIONS; i++)
			{
				static constexpr const char* kConditionNames[] = {
					"", "Menu Open", "On Foot", "Aiming", "Freeroam", "Chatting", "Session Host"
				};
				if (state == kConditionNames[i])
				{
					setCorrelation(click, (ToggleCorrelation::Type)i, invert);
					setStateBasedOnCorrelation(click);
					break;
				}
			}
		}
		else
		{
			removeCorrelation(click);
			setStateBool(click, _state == "On");
		}
	}

	void CommandToggle::applyDefaultState()
	{
		Click click(CLICK_BULK, TC_APPLYDEFAULTSTATE);
		bool should_be_on;
		if (default_correlation.isActive())
		{
			setCorrelation(click, default_correlation);
			should_be_on = correlation.getCurrentValue();
		}
		else
		{
			removeCorrelation(click);
			should_be_on = default_on;
		}
		applyDefaultStateImpl(click, should_be_on);
	}

	void CommandToggle::setCorrelation(Click& click, const ToggleCorrelation& corr)
	{
		setCorrelation(click, corr.type, corr.invert);
	}

	void CommandToggle::setCorrelation(Click& click, ToggleCorrelation::Type corr_type, bool corr_invert)
	{
		correlation.type = corr_type;
		correlation.invert = corr_invert;
		processCorrelationUpdate(click);
	}

	void CommandToggle::removeCorrelation(Click& click)
	{
		correlation.type = ToggleCorrelation::NONE;
		processCorrelationUpdate(click);
	}

	void CommandToggle::processCorrelationUpdate(Click& click)
	{
		auto i = std::find(g_gui.commands_with_correlation.begin(), g_gui.commands_with_correlation.end(), this);
		if (correlation.isActive())
		{
			if (i == g_gui.commands_with_correlation.end())
				g_gui.commands_with_correlation.emplace_back(this);
		}
		else
		{
			if (i != g_gui.commands_with_correlation.end())
				g_gui.commands_with_correlation.erase(i);
		}
	}

	void CommandToggle::setStateBasedOnCorrelation(Click& click)
	{
		if (!correlation.isActive())
			return;
		setStateBasedOnCorrelation(click, ToggleCorrelation::getCurrentValue(correlation.type));
	}

	void CommandToggle::setStateBasedOnCorrelation(Click& click, bool value)
	{
		if (correlation.invert)
			value = !value;
		setStateBool(click, value);
	}

	void CommandToggle::toggleCorrelation(Click& click, bool on, ToggleCorrelation::Type corr_type, bool corr_invert)
	{
		if (on)
		{
			setCorrelation(click, corr_type, corr_invert);
			setStateBasedOnCorrelation(click);
		}
		else
		{
			removeCorrelation(click);
			setStateBool(click, false);
		}
	}
}
