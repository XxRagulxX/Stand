#include "Commands/CommandColourCustom.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
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
