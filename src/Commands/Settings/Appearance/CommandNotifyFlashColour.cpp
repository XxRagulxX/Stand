#include "Commands/CommandColourCustom.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
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
}
