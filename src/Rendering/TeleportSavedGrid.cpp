#include "Rendering/TeleportSavedGrid.hpp"

#include "Commands/Extra/CommandSavedLocations.hpp"
#include "Game/Entity.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/MenuPopup.hpp"
#include "Rendering/Notifications.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"
#include "Game/vector.hpp"

#include <algorithm>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	TeleportSavedGrid::TeleportSavedGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void TeleportSavedGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Save Current Location", Theme::kText));

		auto categoryInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Category", "Default", nullptr);
		m_CategoryInput = categoryInput.get();
		items_draft.push_back(std::move(categoryInput));

		auto locationNameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Location Name", "", nullptr);
		m_LocationNameInput = locationNameInput.get();
		items_draft.push_back(std::move(locationNameInput));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Save Current Location", [this] {
			FiberPool::queueJob([this] {
				if (!m_CategoryInput || !m_LocationNameInput)
					return;

				std::string category = m_CategoryInput->GetValue();
				std::string newLocationName = m_LocationNameInput->GetValue();

				if (newLocationName.empty())
				{
					Notifications::Show("Custom Teleport", "Please enter a valid name", NotificationType::Warning);
					return;
				}

				if (SavedLocations::GetSavedLocationByName(newLocationName))
				{
					Notifications::Show("Custom Teleport", std::format("Location with name {} already exists", newLocationName));
					return;
				}

				SavedLocation teleportLocation;
				Entity teleportEntity = Self::GetPed();
				if (auto vehicle = Self::GetVehicle())
					teleportEntity = vehicle;

				auto coords = teleportEntity.GetPosition();
				teleportLocation.name = newLocationName;
				teleportLocation.x = coords.x;
				teleportLocation.y = coords.y;
				teleportLocation.z = coords.z;
				teleportLocation.yaw = teleportEntity.GetHeading();
				teleportLocation.pitch = 0.0f; // why do we need pitch and roll anyway?
				teleportLocation.roll = 0.0f;
				SavedLocations::SaveNewLocation(category, teleportLocation);
			});
		}));

		// Categories - selecting one writes back into m_CategoryInput,
		// the same shared-variable behaviour the original's own
		// `category` static has (see this class's own header comment).
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search categories",
		    [] {
			    std::vector<std::string> rows;
			    rows.reserve(SavedLocations::GetAllSavedLocations().size());
			    for (auto& [name, locations] : SavedLocations::GetAllSavedLocations())
				    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    if (m_CategoryInput)
				    m_CategoryInput->SetValue(value);
		    }));

		// Locations - double-click to teleport, Shift-click to delete
		// (through a MenuPopup confirm), a plain click does nothing -
		// see this class's own header comment for the parity with the
		// original's own Selectable(..., AllowDoubleClick) handling.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Locations", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    std::vector<std::string> rows;
			    if (!m_CategoryInput)
				    return rows;

			    auto& all = SavedLocations::GetAllSavedLocations();
			    auto it = all.find(m_CategoryInput->GetValue());
			    if (it == all.end())
				    return rows;

			    rows.reserve(it->second.size());
			    for (auto& location : it->second)
				    rows.push_back(location.name);
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool /*ctrl*/, bool shift, bool doubleClick) {
			    if (!m_CategoryInput)
				    return;

			    auto& all = SavedLocations::GetAllSavedLocations();
			    auto it = all.find(m_CategoryInput->GetValue());
			    if (it == all.end())
				    return;

			    auto locationIt = std::find_if(it->second.begin(), it->second.end(), [&value](const SavedLocation& location) {
				    return location.name == value;
			    });
			    if (locationIt == it->second.end())
				    return;

			    if (shift)
			    {
				    auto category = m_CategoryInput->GetValue();
				    auto name = locationIt->name;
				    MenuPopup::Confirm(std::format("Are you sure you want to delete {}?", name), [category, name] {
					    SavedLocations::DeleteSavedLocation(category, name);
				    });
			    }
			    else if (doubleClick)
			    {
				    rage::fvector3 pos{locationIt->x, locationIt->y, locationIt->z};
				    FiberPool::queueJob([pos] {
					    Self::GetPed().TeleportTo(pos);
				    });
			    }
		    }));
	}
}
