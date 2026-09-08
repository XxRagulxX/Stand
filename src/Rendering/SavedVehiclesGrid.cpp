#include "Rendering/SavedVehiclesGrid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Vehicle/CommandSavedVehicles.hpp"
#include "Commands/Commands.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemConditional.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/MenuPopup.hpp"
#include "Rendering/Notifications.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// drawSaveVehicleButton()'s own early-return in the original -
		// Save/Populate Name only make sense (and only showed at all)
		// with a valid vehicle to save.
		bool HasValidVehicle()
		{
			auto vehicle = Self::GetVehicle();
			return static_cast<bool>(vehicle);
		}
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
	SavedVehiclesGrid::SavedVehiclesGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SavedVehiclesGrid::RefreshList()
	{
		FiberPool::queueJob([this] {
			SavedVehicles::RefreshList(m_Folder, m_Folders, m_Files);
		});
	}

	void SavedVehiclesGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spawninsidesavedveh"_J));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh List", [this] {
			RefreshList();
		}));

		// Folder browsing - "Root" (m_Folder == "") prepended to whatever
		// SavedVehicles::RefreshList() last populated m_Folders with;
		// selecting one re-refreshes for that folder, same as the
		// original's own combo's onSelect.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Folder", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search folders",
		    [this] {
			    std::vector<std::string> rows{"Root"};
			    rows.insert(rows.end(), m_Folders.begin(), m_Folders.end());
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    m_Folder = (value == "Root") ? "" : value;
			    RefreshList();
		    }));

		// Saved vehicles in the current folder - selecting one confirms
		// through MenuPopup (the Phase 1 popup primitive), matching the
		// original's own "Are you sure you want to spawn X" modal
		// exactly.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Saved Vehicles", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    return m_Files;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    MenuPopup::Confirm(std::format("Are you sure you want to spawn {}", value), [this, value] {
				    FiberPool::queueJob([this, value] {
					    auto* spawnInside = Commands::GetCommand<CommandToggleLegacy>("spawninsidesavedveh"_J);
					    SavedVehicles::Load(m_Folder, value, spawnInside && spawnInside->GetState());
				    });
			    });
		    }));

		// Save - File Name/New Folder fields always shown (see this
		// class's own header comment for why, unlike the original which
		// hides New Folder outside Root), Populate Name/Save both gated
		// on HasValidVehicle() like the original's shared early-return.
		// Deliberately still GridItemConditional, not watchCondition():
		// m_FileNameInput/m_NewFolderInput below cache their own live-
		// edited text, and watchCondition()'s repopulate-on-change
		// rebuilds this whole populate() call - including those two
		// fields - the moment HasValidVehicle() flips (e.g. getting out
		// of the vehicle mid-edit), discarding whatever the user had
		// typed. Leaving the reserved-blank-space tradeoff here (see
		// GridItemConditional's own class comment) is safer than a real
		// data-loss bug - same reasoning as SavedPlayersGrid's own
		// Player Editor section.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Save", Theme::kText));

		auto fileNameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "File Name", "", nullptr);
		m_FileNameInput = fileNameInput.get();
		items_draft.push_back(std::move(fileNameInput));

		auto newFolderInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "New Folder", "", nullptr);
		m_NewFolderInput = newFolderInput.get();
		items_draft.push_back(std::move(newFolderInput));

		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemButton>(Theme::kContentWidth,
		        kItemH,
		        "Save",
		        [this] {
			        FiberPool::queueJob([this] {
				        if (!m_FileNameInput)
					        return;

				        std::string fileName = m_FileNameInput->GetValue();
				        if (fileName.empty())
				        {
					        Notifications::Show("Saved Vehicles", "Filename empty!", NotificationType::Warning);
					        return;
				        }

				        const bool saveToNewFolder = m_Folder.empty() && m_NewFolderInput && !m_NewFolderInput->GetValue().empty();
				        const std::string targetFolder = saveToNewFolder ? m_NewFolderInput->GetValue() : m_Folder;

				        SavedVehicles::Save(targetFolder, fileName);

				        if (saveToNewFolder)
				        {
					        m_Folder = targetFolder;
					        m_NewFolderInput->SetValue("");
				        }

				        m_FileNameInput->SetValue("");
				        SavedVehicles::RefreshList(m_Folder, m_Folders, m_Files);
			        });
		        }),
		    HasValidVehicle));

		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemButton>(Theme::kContentWidth,
		        kItemH,
		        "Populate Name",
		        [this] {
			        FiberPool::queueJob([this] {
				        auto vehicle = Self::GetVehicle();
				        if (!vehicle || !m_FileNameInput)
					        return;

				        m_FileNameInput->SetValue(vehicle.GetFullName());
			        });
		        }),
		    HasValidVehicle));
	}
}
