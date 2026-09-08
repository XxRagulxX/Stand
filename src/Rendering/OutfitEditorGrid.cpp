#include "Rendering/OutfitEditorGrid.hpp"

#include "Localization/Strings.hpp"
#include "Ped/Outfit.hpp"
#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Generous static clamp for every stepper's own [min,max] - see
		// this class's header comment for why: populate() only runs
		// once, so a stepper can't track drawable_id_max/texture_id_max
		// changing after a later RefreshStats(). The real bounds check
		// (CheckBoundsDrawable/CheckBoundsTexture, against whatever
		// RefreshStats() last read) happens in each row's onChange
		// instead - this just needs to be wide enough it never itself
		// blocks a legitimate step.
		constexpr int kMaxIndex = 200;
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
	OutfitEditorGrid::OutfitEditorGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void OutfitEditorGrid::RefreshStats()
	{
		auto ped = Self::GetPed().GetHandle();

		for (auto& [id, item] : m_Components.items)
		{
			item.drawable_id = PED::GET_PED_DRAWABLE_VARIATION(ped, id);
			item.drawable_id_max = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, id) - 1;
			item.texture_id = PED::GET_PED_TEXTURE_VARIATION(ped, id);
			item.texture_id_max = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, id, item.drawable_id) - 1;
		}

		for (auto& [id, item] : m_Props.items)
		{
			item.drawable_id = PED::GET_PED_PROP_INDEX(ped, id, 0);
			item.drawable_id_max = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(ped, id) - 1;
			item.texture_id = PED::GET_PED_PROP_TEXTURE_INDEX(ped, id);
			item.texture_id_max = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(ped, id, item.drawable_id) - 1;
		}
	}

	void OutfitEditorGrid::RefreshList()
	{
		FiberPool::queueJob([this] {
			Outfit::OutfitEditor::RefreshList(m_Folder, m_Folders, m_Files);
		});
	}

	// Mirrors RenderComponents(): a drawable id change always sends
	// texture 0 to SET_PED_COMPONENT_VARIATION, not the component's
	// current texture_id - matching the original exactly, not "fixed".
	void OutfitEditorGrid::AddComponentDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		for (auto& [id, item] : m_Components.items)
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("{} [0,{}]", item.label, item.drawable_id_max),
			    item.drawable_id,
			    0,
			    kMaxIndex,
			    [this, id](int value) {
				    auto& item = m_Components.items.at(id);
				    item.drawable_id = value;
				    Outfit::OutfitEditor::CheckBoundsDrawable(item, 0);
				    const int drawable_id = item.drawable_id;

				    FiberPool::queueJob([this, id, drawable_id] {
					    auto ped = Self::GetPed().GetHandle();
					    PED::SET_PED_COMPONENT_VARIATION(ped, id, drawable_id, 0, PED::GET_PED_PALETTE_VARIATION(ped, id));
					    RefreshStats();
				    });
			    }));
		}
	}

	// Mirrors RenderComponentsTextures().
	void OutfitEditorGrid::AddComponentTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		for (auto& [id, item] : m_Components.items)
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("{} TEX [0,{}]", item.label, item.texture_id_max),
			    item.texture_id,
			    0,
			    kMaxIndex,
			    [this, id](int value) {
				    auto& item = m_Components.items.at(id);
				    item.texture_id = value;
				    Outfit::OutfitEditor::CheckBoundsTexture(item, 0);
				    const int drawable_id = item.drawable_id;
				    const int texture_id = item.texture_id;

				    FiberPool::queueJob([this, id, drawable_id, texture_id] {
					    auto ped = Self::GetPed().GetHandle();
					    PED::SET_PED_COMPONENT_VARIATION(ped, id, drawable_id, texture_id, PED::GET_PED_PALETTE_VARIATION(ped, id));
					    RefreshStats();
				    });
			    }));
		}
	}

	// Mirrors RenderProps(): -1 clears the prop entirely instead of
	// setting a drawable index, same as the original.
	void OutfitEditorGrid::AddPropDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		for (auto& [id, item] : m_Props.items)
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("{} [-1,{}]", item.label, item.drawable_id_max),
			    item.drawable_id,
			    -1,
			    kMaxIndex,
			    [this, id](int value) {
				    auto& item = m_Props.items.at(id);
				    item.drawable_id = value;
				    Outfit::OutfitEditor::CheckBoundsDrawable(item, -1);
				    const int drawable_id = item.drawable_id;

				    FiberPool::queueJob([this, id, drawable_id] {
					    auto ped = Self::GetPed().GetHandle();
					    if (drawable_id == -1)
						    PED::CLEAR_PED_PROP(ped, id, 1);
					    else
						    PED::SET_PED_PROP_INDEX(ped, id, drawable_id, 0, TRUE, 0);
					    RefreshStats();
				    });
			    }));
		}
	}

	// Mirrors RenderPropsTextures().
	void OutfitEditorGrid::AddPropTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		for (auto& [id, item] : m_Props.items)
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("{} TEX [0,{}]", item.label, item.texture_id_max),
			    item.texture_id,
			    0,
			    kMaxIndex,
			    [this, id](int value) {
				    auto& item = m_Props.items.at(id);
				    item.texture_id = value;
				    Outfit::OutfitEditor::CheckBoundsTexture(item, 0);
				    const int drawable_id = item.drawable_id;
				    const int texture_id = item.texture_id;

				    FiberPool::queueJob([this, id, drawable_id, texture_id] {
					    auto ped = Self::GetPed().GetHandle();
					    PED::SET_PED_PROP_INDEX(ped, id, drawable_id, texture_id, TRUE, 0);
					    RefreshStats();
				    });
			    }));
		}
	}

	void OutfitEditorGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh Stats", [this] {
			FiberPool::queueJob([this] {
				RefreshStats();
			});
		}));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Randomize Outfit", [] {
			FiberPool::queueJob([] {
				Self::GetPed().RandomizeOutfit2();
			});
		}));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Components", Theme::kText));
		AddComponentDrawableRows(items_draft);

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Component Textures", Theme::kText));
		AddComponentTextureRows(items_draft);

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Props", Theme::kText));
		AddPropDrawableRows(items_draft);

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Prop Textures", Theme::kText));
		AddPropTextureRows(items_draft);

		// Folder browsing - "Root" (m_Folder == "") prepended to
		// whatever Outfit::OutfitEditor::RefreshList() last populated
		// m_Folders with, same pattern as SavedVehiclesGrid's own.
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

		// Saved outfits in the current folder - selecting one just
		// records it (matching the original's own onSelect, which only
		// sets file); a separate "Apply Selected Outfit" button below
		// actually applies it, unlike SavedVehiclesGrid's own list,
		// which applies immediately through a confirm popup.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Saved Outfits", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    return m_Files;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    m_File = value;
		    }));

		// Load/save controls - File Name/New Folder always shown (same
		// deliberate deviation from the original as SavedVehiclesGrid's
		// own - see that class's header comment for why), Apply hair via
		// GridItemBoundToggle bound to m_ApplyHair, reset after every
		// apply same as the original's own static bool.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Load / Save", Theme::kText));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh List", [this] {
			RefreshList();
		}));

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Apply hair",
		    [this] {
			    return m_ApplyHair;
		    },
		    [this](bool value) {
			    m_ApplyHair = value;
		    }));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Apply Selected Outfit", [this] {
			FiberPool::queueJob([this] {
				Outfit::OutfitEditor::ApplyOutfitFromJson(m_Folder, m_File, m_ApplyHair);
				m_ApplyHair = false;
				RefreshStats();
			});
		}));

		auto outfitNameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Outfit Name", "", nullptr);
		m_OutfitNameInput = outfitNameInput.get();
		items_draft.push_back(std::move(outfitNameInput));

		auto newFolderInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "New Folder", "", nullptr);
		m_NewFolderInput = newFolderInput.get();
		items_draft.push_back(std::move(newFolderInput));

		// Save - resolves the actual save destination (targetFolder)
		// before saving into it, same fix SavedVehiclesGrid's own Save
		// button already applies relative to its own original source
		// (which saves into the *old* folder even when a New Folder
		// name was given, then only switches folder = newFolder
		// afterwards - a real bug, not a deliberate quirk, so not
		// preserved here either).
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Save Outfit", [this] {
			FiberPool::queueJob([this] {
				if (!m_OutfitNameInput)
					return;

				std::string raw = m_OutfitNameInput->GetValue();
				std::string fileName = TrimString(raw);

				if (fileName.empty())
				{
					Notifications::Show("Outfit", "Filename empty!", NotificationType::Warning);
					return;
				}

				const bool saveToNewFolder = m_Folder.empty() && m_NewFolderInput && !m_NewFolderInput->GetValue().empty();
				const std::string targetFolder = saveToNewFolder ? m_NewFolderInput->GetValue() : m_Folder;

				Outfit::OutfitEditor::SaveOutfit(fileName, targetFolder);

				if (saveToNewFolder)
				{
					m_Folder = targetFolder;
					m_NewFolderInput->SetValue("");
				}

				m_OutfitNameInput->SetValue("");
				Outfit::OutfitEditor::RefreshList(m_Folder, m_Folders, m_Files);
			});
		}));
	}
}
