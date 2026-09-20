#include "Rendering/SettingsProfilesGrid.hpp"

#include "Commands/Settings/Profiles/CommandTabProfiles.hpp"
#include "Config/ProfileManager.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsProfilesGrid::SettingsProfilesGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsProfilesGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		const auto capturedGen = ProfileManager::GetGeneration();
		watchCondition([capturedGen] {
			return ProfileManager::GetGeneration() == capturedGen;
		});

		items_draft.push_back(std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "New Profile Name", "", [](const std::string& name) {
			if (!name.empty())
				ProfileManager::SaveProfile(name);
		}));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, Features::GetCommandTabProfiles().openProfilesFolder));

		const auto profiles = ProfileManager::ListProfiles();
		if (!profiles.empty())
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Profiles", Theme::kText));

			for (const auto& name : profiles)
			{
				items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, name, Theme::kText));

				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Load", [name] {
					ProfileManager::LoadProfile(name);
					Notifications::Show("StandEnhanced", "Profile \"" + name + "\" loaded.");
				}));

				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Delete", [this, name] {
					ProfileManager::DeleteProfile(name);
					Notifications::Show("StandEnhanced", "Profile \"" + name + "\" deleted.");
					invalidate();
				}));
			}
		}
	}
}
