#include "Commands/CommandColourCustom.hpp"
#include "Commands/Widgets/CommandRainbow.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandPrimaryColour (Commands/Stand/
	// CommandPrimaryColour.hpp on origin/stand-reference) - the one
	// accent colour used everywhere in this menu (Theme::kAccent, this
	// project's own equivalent of real Stand's focusRectColour: a
	// sidebar/tab's active entry, a toggle's ON state, a button's fill).
	// Same default both real Stand's own class and this project's own
	// Theme::kAccent already use - {1, 0, 1, 1} (pink).
	class CommandPrimaryColour : public CommandColourCustom
	{
	public:
		CommandPrimaryColour() :
		    CommandColourCustom("primary",
		        "Primary Colour",
		        "The one accent colour used everywhere in this menu - a sidebar/tab's active entry, a toggle's ON state, a button's fill.",
		        ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		// Syncs Theme::kAccent from a saved config value too, not just a
		// live edit - OnChange() above only ever fires from SetState()
		// (see CommandColourCustom::SetState()'s own FiberPool::queueJob call),
		// which a config load never goes through (LoadState() writes
		// m_State directly, bypassing SetState() entirely) - the same
		// "loaded but never applied" gap this project's own CommandToggle
		// already has a dedicated fix for (Commands::EnableBoolCommandsImpl()).
		// CommandColourCustom has no generic equivalent, so this command fixes
		// it locally instead.
		void LoadState(nlohmann::json& value) override
		{
			CommandColourCustom::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kAccent = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandPrimaryColour _PrimaryColour{};

	// Nested under Primary Colour in real Stand's own tree (its
	// CommandTabStand.cpp creates this as primaryColour's own child) -
	// see CommandRainbow.hpp's own class comment for what's simplified
	// from real Stand's version (no cross-updating a focused/unfocused
	// text colour split this renderer doesn't have yet).
	static StandWidgets::CommandRainbow _PrimaryColourRainbow{"rainbow",
	    "Rainbow Mode",
	    "Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity.",
	    &Rendering::Theme::kAccent};
}
