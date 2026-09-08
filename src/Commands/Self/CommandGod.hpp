#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "World/Self.hpp"

// First real feature migrated onto the ported Stand tree (the "Self"
// category pilot - see this file's own construction site,
// Commands/Self/CommandGod.cpp, for why it isn't parented into a real
// CommandList yet). Replaces the legacy Commands/Self/CommandGodmode.cpp
// (Stand::Features::Godmode, a LoopedCommand) - SelfGrid.cpp's own
// "godmode" row now points here instead (via GridItemStandCommand, not
// GridItemCommandToggle - see that row's own comment in SelfGrid.cpp).
//
// Matches real Stand's own CommandGod (Commands/Self/CommandGod.hpp on
// origin/stand-reference) for command_names/menu_name/help_text:
//   explicit CommandGod(CommandList* const parent)
//       : CommandToggle(parent, LOC("GOD"), { CMDNAME("godmode"), CMDNAME("immortality") }, LOC("GOD_H"))
// LOC("GOD")/LOC("GOD_H") are translation-table KEYS, not the displayed
// text itself - real Stand's own English text for them (confirmed
// against its own reference screenshot) is "Immortality" and "Makes
// your character unable to die.", used directly here via LIT() since
// this project has no translation table of its own to resolve a LOC key
// through (see Util/Label.hpp's own class comment) - an earlier pass
// here wrongly guessed "God Mode" instead of actually checking what
// real Stand displays. Real Stand's own onEnable()/
// onDisable() just flip its own g_gui.godmode flag, with the actual
// SetInvincible-equivalent enforcement happening every tick somewhere
// else entirely in its own renderer (not ported) - this project has no
// such "somewhere else" to hook into, so onEnable()/onDisable()/onTick()
// below instead port the legacy Godmode.cpp's own LoopedCommand shape
// directly onto CommandTickDispatch (see that class's own doc comment
// for why): apply Self::GetPed().SetInvincible() immediately on
// enable/disable for instant feedback, same as the legacy version's own
// OnDisable(), and keep re-asserting it every tick while on (forcing it
// back off the instant the ped's actually dead, then back on again once
// alive - e.g. after a respawn) for as long as this stays toggled on,
// exactly the safety behaviour legacy Godmode.cpp's own OnTick() had.
namespace Stand
{
	class CommandGod : public CommandToggle
	{
	public:
		explicit CommandGod(CommandList* parent) :
		    CommandToggle(parent, LIT("Immortality"), CMDNAMES("godmode", "immortality"), LIT("Makes your character unable to die."))
		{
		}

		void onEnable(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetInvincible(true);

			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);

			if (auto ped = Stand::Self::GetPed())
				ped.SetInvincible(false);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			ped.SetInvincible(!ped.IsDead());
		}

		~CommandGod() override
		{
			// Defensive - onDisable() already removes this on a normal
			// user-driven toggle-off, but a still-enabled instance being
			// destroyed (nothing does this today; every real instance is
			// a function-local static that outlives the process) should
			// never leave a dangling pointer behind in the dispatcher.
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

namespace Stand::Features
{
	// The one real instance - see CommandGod.cpp for why this is a
	// function-local static behind an accessor rather than a plain
	// namespace-scope global. SelfGrid.cpp's own "godmode" row calls
	// this to get the Stand::Command* GridItemStandCommand needs.
	Stand::CommandGod& GetCommandGod();
}
