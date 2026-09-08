#include "Commands/CommandColourCustom.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyBgColour (Commands/
	// Online/CommandListNotifySettings.cpp on origin/stand-reference) -
	// defaults to a copy of Settings > Appearance > Background Colour's
	// own default ((0, 0, 0, 77/255)) - see CommandNotifyBorderColour.cpp's
	// own comment for why there's no literal copy-from-Background-Colour
	// BUTTON widget, only the matching default value. No Rainbow Mode -
	// real Stand's own tree doesn't list one for this colour either.
	class CommandNotifyBackgroundColour : public CommandColourCustom
	{
	public:
		CommandNotifyBackgroundColour() :
		    CommandColourCustom("notifybg", "Background Colour", "The notification card's own translucent background.", ImVec4(0.f, 0.f, 0.f, 77.f / 255.f))
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
			Rendering::NotifySettings::kBackgroundColour = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandNotifyBackgroundColour _NotifyBackgroundColour{};
}
