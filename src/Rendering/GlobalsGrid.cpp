#include "Rendering/GlobalsGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/SavedVariableGridRows.hpp"
#include "Rendering/Theme.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Mirrors Core/Globals.cpp's own static SaveGlobal()/DeleteGlobal()
		// exactly - upsert/erase by name.
		void SaveGlobal(SavedGlobal& globalObj)
		{
			for (auto& global : SavedVariables::GetSavedGlobals())
			{
				if (global.name == globalObj.name)
				{
					global = globalObj;
					SavedVariables::Save();
					return;
				}
			}

			SavedVariables::GetSavedGlobals().push_back(globalObj);
			SavedVariables::Save();
		}

		void DeleteGlobal(const SavedGlobal& globalObj)
		{
			std::erase_if(SavedVariables::GetSavedGlobals(), [&](SavedGlobal& g) {
				return g.name == globalObj.name;
			});
			SavedVariables::Save();
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
	GlobalsGrid::GlobalsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void GlobalsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Deliberately not in the constructor: this Grid is a file-scope
		// static (see MiscGrid.cpp's own instance), so its constructor
		// runs during C++ static initialization, before main() calls
		// FileMgr::Init() - SavedVariables::Init() reading
		// saved_variables.json through FileMgr that early would resolve
		// against FileMgr's still-default-constructed (empty) root
		// folder, silently loading nothing. populate() only runs lazily,
		// on this Grid's first real draw - long after FileMgr::Init()
		// has already run - so it belongs here instead; still only-once
		// thanks to SavedVariables::Init()'s own m_Initialized guard.
		SavedVariables::Init();

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Editor", Theme::kText));
		AddSavedVariableDefinitionRows(items_draft, Theme::kContentWidth, m_CurGlobal, [this] {
			invalidate();
		});
		AddSavedVariableValueEditRows(items_draft, Theme::kContentWidth, kItemH, m_CurGlobal, [this] {
			return m_CurGlobal.Read();
		});

		// Saved - one row per saved global, its live value folded into
		// the row's own label (see SavedVariableGridRows.hpp's own
		// comment on FormatSavedVariablePreview for why there's one list
		// here rather than the original's own two side by side).
		// Selecting one loads it into the editor above, same as the
		// original's own onSelect - a shape change (Type/appendages), so
		// it invalidates.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Saved", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [] {
			    std::vector<std::string> rows;
			    auto& saved = SavedVariables::GetSavedGlobals();
			    rows.reserve(saved.size());
			    for (auto& var : saved)
				    rows.push_back(std::format("{} = {}", var.name, FormatSavedVariablePreview(var, var.Read())));
			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    auto& saved = SavedVariables::GetSavedGlobals();
			    if (index >= saved.size())
				    return;

			    m_CurGlobal = saved[index];
			    if (m_NameInput)
				    m_NameInput->SetValue(m_CurGlobal.name);
			    invalidate();
		    }));

		auto nameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Name", m_CurGlobal.name, nullptr);
		m_NameInput = nameInput.get();
		items_draft.push_back(std::move(nameInput));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Save", [this] {
			if (!m_NameInput)
				return;

			m_CurGlobal.name = m_NameInput->GetValue();
			SaveGlobal(m_CurGlobal);
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Delete", [this] {
			if (m_NameInput)
				m_CurGlobal.name = m_NameInput->GetValue();

			DeleteGlobal(m_CurGlobal);

			m_CurGlobal = SavedGlobal();
			if (m_NameInput)
				m_NameInput->SetValue("");
			invalidate();
		}));
	}
}
