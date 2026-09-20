#include "Commands/CommandColourCustom.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
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
}
