#include "Commands/CommandColourCustom.hpp"
#include "Commands/Widgets/CommandRainbow.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandBorderColour : public CommandColourCustom
	{
	public:
		CommandBorderColour() :
		    CommandColourCustom("border", "Colour", "The colour of the item border.", ImVec4(1.f, 1.f, 1.f, 1.f))
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
			Rendering::Theme::kBorderColour = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandBorderColour _BorderColour{};

	static StandWidgets::CommandRainbow _BorderColourRainbow{"borderrainbow",
	    "Rainbow Mode",
	    "Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity.",
	    &Rendering::Theme::kBorderColour};
}
