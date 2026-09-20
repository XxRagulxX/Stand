#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandBackgroundColour (Commands/
	// Stand/CommandBackgroundColour.hpp on origin/stand-reference) - the
	// translucent panel background every non-focused row sits on
	// (Theme::kPanelBackground, this project's own equivalent of real
	// Stand's bgRectColour). Same default both real Stand's own class
	// and this project's own Theme::kPanelBackground already use -
	// (0, 0, 0, 77/255) i.e. black at ~30% opacity.
	class CommandBackgroundColour : public CommandColourCustom
	{
	public:
		CommandBackgroundColour() :
		    CommandColourCustom("background",
		        "Background Colour",
		        "The translucent panel background every non-focused row sits on.",
		        ImVec4(0.f, 0.f, 0.f, 77.f / 255.f))
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		// Same reasoning as CommandPrimaryColour.cpp's own LoadState()
		// override - keeps Theme::kPanelBackground in sync with a saved
		// config value too, not just a live edit.
		void LoadState(nlohmann::json& value) override
		{
			CommandColourCustom::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kPanelBackground = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandBackgroundColour _BackgroundColour{};
}
