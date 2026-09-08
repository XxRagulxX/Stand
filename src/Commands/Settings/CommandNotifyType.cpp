#include "Commands/CommandListSelect.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyType (Commands/Online/
	// CommandListNotifySettings.cpp on origin/stand-reference) - which of
	// its three notification styles is active: Game routes straight
	// through the native GTA5 feed, the other two draw this project's
	// own DirectXTK12 toast overlay (Notifications.cpp) anchored either
	// next to the minimap or at a user-set custom position. Default
	// index 1 ("Stand, Next To Map") matches real Stand's own default.
	class CommandNotifyType : public CommandListSelect
	{
	public:
		CommandNotifyType() :
		    CommandListSelect("notifytype",
		        "Type",
		        "Which notification style to use - the native GTA5 feed, or this menu's own overlay next to the minimap or at a custom position.",
		        {{0, "Game"}, {1, "Stand, Next To Map"}, {2, "Stand, Custom Position"}},
		        1)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		// Same "a loaded config value needs syncing too, not just a live
		// edit" reasoning as every other Settings command in this
		// feature - CommandListSelect::LoadState() writes m_State directly,
		// bypassing SetState()/OnChange() entirely.
		void LoadState(nlohmann::json& value) override
		{
			CommandListSelect::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			switch (GetState())
			{
			case 0:
				Rendering::NotifySettings::kType = Rendering::NotifySettings::Type::Game;
				break;
			case 2:
				Rendering::NotifySettings::kType = Rendering::NotifySettings::Type::StandCustomPosition;
				break;
			default:
				Rendering::NotifySettings::kType = Rendering::NotifySettings::Type::StandNextToMap;
				break;
			}
		}
	};

	static CommandNotifyType _NotifyType{};
}
