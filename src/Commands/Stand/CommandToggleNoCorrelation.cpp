#include "Commands/Stand/CommandToggleNoCorrelation.hpp"

#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	void CommandToggleNoCorrelation::onClick(Click& click)
	{
		click.sound_feedback = m_on ? SOUND_NO : SOUND_YES;
		toggleState(click);
		onChange(click);
	}

	std::string CommandToggleNoCorrelation::getCommandSyntax() const
	{
		if (command_names.empty())
			return {};

		return CommandPhysical::getCommandSyntax() + " [on/off]";
	}

	void CommandToggleNoCorrelation::onChange(Click& click)
	{
		if (m_on)
			onEnable(click);
		else
			onDisable(click);
	}

	void CommandToggleNoCorrelation::onEnable(Click& click)
	{
	}

	void CommandToggleNoCorrelation::onDisable(Click& click)
	{
	}

	std::string CommandToggleNoCorrelation::getState() const
	{
		return m_on ? "On" : "Off";
	}

	std::string CommandToggleNoCorrelation::getDefaultState() const
	{
		return default_on ? "On" : "Off";
	}

	void CommandToggleNoCorrelation::setState(Click& click, const std::string& state)
	{
		setStateBool(click, state == "On");
	}

	void CommandToggleNoCorrelation::applyDefaultState()
	{
		Click click(CLICK_BULK, TC_SCRIPT_YIELDABLE);
		if (m_on != default_on)
			onClick(click);
	}

	void CommandToggleNoCorrelation::setStateBool(Click& click, bool toggle)
	{
		if (m_on != toggle)
			onClick(click);
	}

	void CommandToggleNoCorrelation::toggleState(Click& click)
	{
		m_on = !m_on;
		updateState(click);
	}

	void CommandToggleNoCorrelation::onChangeToggleScriptTickEventHandler(Click& click, std::function<bool()>&& handler)
	{
		if (m_on)
		{
			m_tickHandler = std::move(handler);
			CommandTickDispatch::AddCommand(this);
		}
	}

	void CommandToggleNoCorrelation::updateState(Click& click)
	{
		if (click.canHaveGenericResponse())
			click.setGenericResponse(LIT(getMenuName().getLocalisedUtf8() + (m_on ? " is now enabled" : " is now disabled")));

		CommandStateSerializer::MarkDirty();
	}
}
