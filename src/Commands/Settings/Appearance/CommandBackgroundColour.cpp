#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
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
