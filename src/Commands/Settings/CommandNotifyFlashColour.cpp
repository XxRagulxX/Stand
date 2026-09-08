#include "Commands/CommandColourCustom.hpp"
#include "Commands/Widgets/CommandRainbow.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyFlashColour (Commands/
	// Online/CommandListNotifySettings.cpp on origin/stand-reference) -
	// unlike Border/Background Colour, this has no "copy from" default;
	// its own distinct default (a magenta/purple, matching real Stand's
	// own {0.6196, 0, 0.6196, 1}) - see Notifications.cpp's own
	// DrawNotificationRect() for where this shows (briefly replacing
	// Border Colour right after a notification first appears or
	// re-triggers).
	class CommandNotifyFlashColour : public CommandColourCustom
	{
	public:
		CommandNotifyFlashColour() :
		    CommandColourCustom("notifyflash", "Flash Colour", "Briefly shown in place of Border Colour right after a notification first appears or re-triggers.", ImVec4(0.6196f, 0.f, 0.6196f, 1.f))
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
			Rendering::NotifySettings::kFlashColour = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandNotifyFlashColour _NotifyFlashColour{};

	static StandWidgets::CommandRainbow _NotifyFlashColourRainbow{"notifyflashrainbow",
	    "Rainbow Mode",
	    "Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity.",
	    &Rendering::NotifySettings::kFlashColour};
}
