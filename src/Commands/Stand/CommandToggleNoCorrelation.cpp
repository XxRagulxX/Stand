#include "Commands/Stand/CommandToggleNoCorrelation.hpp"

#include "Commands/Widgets/CommandStateSerializer.hpp"
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

	void CommandToggleNoCorrelation::updateState(Click& click)
	{
		// Real Stand's own equivalent just sets a bare "On"/"Off" - this
		// project's own request instead wants the toast to name the
		// feature itself (e.g. "Immortality is now enabled"), since a
		// bare "On"/"Off" toast is meaningless without already knowing
		// which row you just clicked/hotkeyed/typed into the command
		// console. Only fires when this actually reaches
		// Notifications::Show() at all - see Click::respond()'s own
		// gating (canHaveResponse()/non-empty response) and, more
		// importantly, every Click-producing call site that actually has
		// to call ensureResponse()+respond() itself for this to show up
		// (GridItemStandCommand.cpp's own ToggleClicked(), MenuCommandConsole.cpp's
		// own Stand-command activation, CommandHotkeyDispatch.cpp already
		// did) - a real, previously-missing wire-up this project's own
		// request surfaced, not something real Stand's own source needed
		// (its own menu-click/hotkey dispatch already always calls
		// respond() generically).
		if (click.canHaveGenericResponse())
			click.setGenericResponse(LIT(getMenuName().getLocalisedUtf8() + (m_on ? " is now enabled" : " is now disabled")));

		// Every path that actually changes m_on (toggleState() from a
		// real click, setStateBool() from setState()/applyDefaultState())
		// funnels through here - see CommandStateSerializer.hpp's own
		// class comment for why persistence is driven off dirty-marking
		// rather than saving unconditionally every tick.
		CommandStateSerializer::MarkDirty();
	}
}
