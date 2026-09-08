#pragma once
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"

// Real Stand's CommandToggle adds ToggleCorrelation (auto-enable/disable
// based on live player state - menu open, on foot, aiming, freeroam,
// session host) on top of CommandToggleNoCorrelation, plus a second
// constructor overload that takes a ToggleCorrelation::Type directly.
// Not ported yet - it needs Core/ToggleCorrelation.hpp, which itself
// needs a real shim from Stand's own AbstractEntity/g_player_ped onto
// this project's own Self::GetPed()/natives, a decision that shouldn't
// be faked with a stub. CommandTabSelf.cpp's own Self-tab content
// doesn't use the correlation constructor anywhere, so this is a plain
// alias-shaped subclass for now: exists so a ported command's
// `CommandToggle(...)` constructor call compiles unchanged, with the
// same toggle behaviour as CommandToggleNoCorrelation and no
// auto-correlation. Revisit once a feature actually needs it.
namespace Stand
{
	class CommandToggle : public CommandToggleNoCorrelation
	{
	public:
		explicit CommandToggle(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, bool default_on = false, commandflags_t flags = CMDFLAGS_TOGGLE, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}) :
		    CommandToggleNoCorrelation(parent, std::move(menu_name), std::move(command_names), std::move(help_text), default_on, flags, perm, default_hotkeys, COMMAND_TOGGLE)
		{
		}
	};
}
