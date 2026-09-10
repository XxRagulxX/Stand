#include "Rendering/SettingsProfilesGrid.hpp"

#include "Config/ProfileManager.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandInput.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

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

		items_draft.push_back(std::make_unique<GridItemCommandInput>(Theme::kContentWidth, kItemH, "newprofile"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "openprofilesfolder"_J));

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
