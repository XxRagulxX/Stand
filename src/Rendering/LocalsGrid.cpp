#include "Rendering/LocalsGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/SavedVariableGridRows.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/scrThread.hpp"
#include "Util/Joaat.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Mirrors Scripting/Locals.cpp's own static SaveLocal()/
		// DeleteLocal() exactly - upsert/erase by (name, script).
		void SaveLocal(SavedLocal& localObj)
		{
			for (auto& local : SavedVariables::GetSavedLocals())
			{
				if (local.name == localObj.name && local.script == localObj.script)
				{
					local = localObj;
					SavedVariables::Save();
					return;
				}
			}

			SavedVariables::GetSavedLocals().push_back(localObj);
			SavedVariables::Save();
		}

		void DeleteLocal(const SavedLocal& localObj)
		{
			std::erase_if(SavedVariables::GetSavedLocals(), [&](SavedLocal& g) {
				return g.name == localObj.name && g.script == localObj.script;
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
	LocalsGrid::LocalsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void LocalsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Deliberately not in the constructor - see GlobalsGrid.cpp's
		// identical note for why (this Grid is also a file-scope static,
		// constructed before FileMgr::Init() has run).
		SavedVariables::Init();

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Script", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemTextInput>(Theme::kContentWidth,
		    kItemH,
		    "Script",
		    m_ScriptName,
		    [this](const std::string& value) {
			    m_ScriptName = value;
			    m_CurThread = Scripts::FindScriptThread(Joaat(value));
			    m_CurLocal = SavedLocal();
			    m_CurLocal.script = m_CurThread ? m_CurThread->m_ScriptHash : 0;
			    invalidate();
		    }));

		if (!m_CurThread)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Invalid", Theme::kError));
			return;
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Editor", Theme::kText));
		AddSavedVariableDefinitionRows(items_draft, Theme::kContentWidth, m_CurLocal, [this] {
			invalidate();
		});
		AddSavedVariableValueEditRows(items_draft, Theme::kContentWidth, kItemH, m_CurLocal, [this] {
			return m_CurThread ? m_CurLocal.Read(m_CurThread) : nullptr;
		});

		// Saved - filtered to the current script, same as the original's
		// own two list boxes; its live value folded into the row's own
		// label (see SavedVariableGridRows.hpp's own comment on
		// FormatSavedVariablePreview). onSelect rebuilds the identical
		// filtered list to map index back to the right SavedLocal -
		// index is only ever meaningful against rows()'s own result for
		// this call (see GridItemSelectList's own class comment), not
		// the raw unfiltered SavedVariables::GetSavedLocals().
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Saved", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    std::vector<std::string> rows;
			    if (!m_CurThread)
				    return rows;

			    for (auto& var : SavedVariables::GetSavedLocals())
				    if (var.script == m_CurThread->m_ScriptHash)
					    rows.push_back(std::format("{} = {}", var.name, FormatSavedVariablePreview(var, var.Read(m_CurThread))));

			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    if (!m_CurThread)
				    return;

			    std::vector<SavedLocal*> filtered;
			    for (auto& var : SavedVariables::GetSavedLocals())
				    if (var.script == m_CurThread->m_ScriptHash)
					    filtered.push_back(&var);

			    if (index >= filtered.size())
				    return;

			    m_CurLocal = *filtered[index];
			    if (m_NameInput)
				    m_NameInput->SetValue(m_CurLocal.name);
			    invalidate();
		    }));

		auto nameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Name", m_CurLocal.name, nullptr);
		m_NameInput = nameInput.get();
		items_draft.push_back(std::move(nameInput));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Save", [this] {
			if (!m_NameInput)
				return;

			m_CurLocal.name = m_NameInput->GetValue();
			SaveLocal(m_CurLocal);
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Delete", [this] {
			if (m_NameInput)
				m_CurLocal.name = m_NameInput->GetValue();

			DeleteLocal(m_CurLocal);

			// Restores script onto the freshly-reset SavedLocal - the
			// original's own Delete handler doesn't (curLocal = SavedLocal()
			// only), but its own "Saved" block re-derives it every single
			// frame right after (comparing scriptIdf against curLocal.script
			// and correcting it there), which this class has no equivalent
			// of - see this class's own header comment on why a repopulate
			// only ever happens on a discrete action here, not a per-frame
			// poll. Restoring it immediately reaches the exact same
			// end state the original does one frame later, so a "Save" right
			// after "Delete" still saves under the right script.
			const auto script = m_CurLocal.script;
			m_CurLocal = SavedLocal();
			m_CurLocal.script = script;
			if (m_NameInput)
				m_NameInput->SetValue("");
			invalidate();
		}));
	}
}
