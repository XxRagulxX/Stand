#include "Commands/CommandColourCustom.hpp"
#include "Commands/Widgets/CommandRainbow.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyBorderColour (Commands/
	// Online/CommandListNotifySettings.cpp on origin/stand-reference) -
	// defaults to a copy of Primary Colour's own default ({1, 0, 1, 1}),
	// same as real Stand's own "Copy Primary Colour" default value. No
	// literal copy-from-Primary-Colour BUTTON widget - this project's
	// CommandColourCustom/GridItemCommandColourCustom has no "copy another command's
	// current value" action mechanism yet, so only the matching default
	// is ported, a disclosed simplification (same trade-off already made
	// for Background Colour below, and for Settings > Appearance's own
	// Background Colour earlier).
	class CommandNotifyBorderColour : public CommandColourCustom
	{
	public:
		CommandNotifyBorderColour() :
		    CommandColourCustom("notifyborder", "Border Colour", "The notification card's own border/remaining-time colour.", ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandColourCustom::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::NotifySettings::kBorderColour = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandNotifyBorderColour _NotifyBorderColour{};

	static StandWidgets::CommandRainbow _NotifyBorderColourRainbow{"notifyborderrainbow",
	    "Rainbow Mode",
	    "Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity.",
	    &Rendering::NotifySettings::kBorderColour};
}
