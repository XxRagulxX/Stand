#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "World/Self.hpp"

// Second real feature migrated onto the ported Stand tree (Self, after
// Commands/Self/CommandGod.hpp). Replaces the legacy Commands/Self/
// CommandAutoHeal.cpp (Stand::Features::AutoHeal, a LoopedCommand) -
// SelfGrid.cpp's own "autoheal" row now points here instead (via
// GridItemStandCommand, not GridItemCommandToggle).
//
// Matches real Stand's own CommandAutoHeal (Commands/Self/
// CommandAutoHeal.hpp on origin/stand-reference) for command_names/
// class name/logic:
//   explicit CommandAutoHeal(CommandList* const parent)
//       : CommandToggle(parent, LOC("DEMIGOD"), { CMDNAME("demigodmode"), CMDNAME("semigodmode") }, LOC("DEMIGOD_H"))
// menu_name/help_text below ("Demi-God Mode" / a description matching
// the legacy version's own "Keeps your health and armour topped up") -
// UNLIKE CommandGod's own "GOD"/"GOD_H" keys, this project has no
// screenshot or string table to confirm real Stand's own actual English
// text for "DEMIGOD"/"DEMIGOD_H" against (this repo's own
// origin/stand-reference branch carries no localisation/lang table at
// all - see Util/Label.hpp's own class comment) - "Demi-God Mode" is a
// reasoned inference from the LOC key and the "demigodmode"/
// "semigodmode" aliases themselves, not a confirmed value the way
// "Immortality" was. Flagged, not silently guessed as fact this time -
// correct it if it's wrong.
//
// Real Stand's own onChange() registers a one-shot script-tick handler
// (onChangeToggleScriptTickEventHandler(), tied to its own TickMgr - not
// ported, see CommandPhysical.hpp's own class comment) that keeps
// re-running the health/armour top-up for as long as this stays on,
// rather than a plain one-time onEnable()/onDisable() pair - this
// project has no such "register a handler" primitive, so this instead
// follows the same CommandTickDispatch pattern CommandGod.hpp already
// established (onEnable() registers, onDisable() unregisters, onTick()
// does the actual work) - same end result (health/armour never sit
// below max while this is on), just via this project's own tick shape.
namespace Stand
{
	class CommandAutoHeal : public CommandToggle
	{
	public:
		explicit CommandAutoHeal(CommandList* parent) :
		    CommandToggle(parent, LIT("Demi-God Mode"), CMDNAMES("demigodmode", "semigodmode"), LIT("Keeps your health and armour topped up."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
			onTick();
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!Stand::Self::GetPed())
				return;

			const auto health = Stand::Self::GetPed().GetHealth();
			const auto maxHealth = Stand::Self::GetPed().GetMaxHealth();
			if (health > 0 && health < maxHealth)
				Stand::Self::GetPed().SetHealth(maxHealth);

			const auto maxArmour = Stand::Self::GetPlayer().GetMaxArmour();
			if (Stand::Self::GetPed().GetArmour() != maxArmour)
				Stand::Self::GetPed().SetArmour(maxArmour);
		}

		~CommandAutoHeal() override
		{
			// Defensive - see CommandGod.hpp's own destructor comment.
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

namespace Stand::Features
{
	// The one real instance - see Commands/Self/CommandAutoHeal.cpp and
	// CommandGod.cpp's own comment for why this is a function-local
	// static behind an accessor rather than a plain namespace-scope
	// global. SelfGrid.cpp's own "autoheal" row calls this to get the
	// Stand::Command* GridItemStandCommand needs.
	Stand::CommandAutoHeal& GetCommandAutoHeal();
}
