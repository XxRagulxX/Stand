#include "Commands/CommandSliderLegacy.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's CommandMaxHealth is a single slider, 200-15000 step 25,
	// where 200 (its own floor) means "Don't Override" - not applied at
	// all, restoring whatever the game/rank system would normally set.
	// Every other CommandSlider in this codebase is a plain value holder
	// with a separate LoopedCommand actually applying it each tick (see
	// CommandWanted.cpp's own wantedslider/freezewanted split) - there's
	// no real Stand-style "continuously re-applied slider" idiom here.
	// This applies the value once, immediately, whenever it changes
	// (CommandSlider::OnChange() already runs on the game script thread via
	// FiberPool - see CommandSlider::SetState()) rather than re-asserting it
	// every tick - a real, disclosed gap from Stand's own persistence
	// (something else resetting max health later, e.g. on respawn,
	// wouldn't get immediately overridden the way Stand's own tick
	// handler would), but keeps this a single row rather than a second
	// "lock" toggle nothing else in Stand's own UI has either.
	class MaxHealth : public CommandSliderLegacy
	{
		using CommandSliderLegacy::CommandSliderLegacy;

		virtual void OnChange() override
		{
			if (!Self::GetPed())
				return;

			const auto value = GetState();
			if (value <= 200)
				return; // "Don't Override" - leave whatever the game already set alone

			ENTITY::SET_ENTITY_MAX_HEALTH(Self::GetPed().GetHandle(), value);
		}
	};

	static MaxHealth _MaxHealth{"maxhealth", "Maximum Health", "Sets your maximum health (200 = don't override)", 200, 15000, 200};
}
