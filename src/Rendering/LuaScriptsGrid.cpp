#include "Rendering/LuaScriptsGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemLiveText.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Scripting/LuaManager.hpp"
#include "Rendering/Theme.hpp"

#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
		constexpr std::string_view kUnloadedPrefix = "[Unloaded] ";

		// ForAllLoadedScripts()/ForAllUnloadedScripts() (LuaManager.hpp)
		// both take a plain function pointer, not a std::function - no
		// capturing lambda can be passed directly, so every callback
		// below is captureless and reads/writes file-static scratch
		// state instead (same reason the original's own callbacks read
		// a static local `selectedScript` rather than capturing
		// anything).
		std::vector<std::string> g_LoadedNames;
		std::vector<std::pair<std::string, std::string>> g_UnloadedScripts; // (name, path)

		void CollectLoadedName(std::shared_ptr<LuaScript>& script)
		{
			g_LoadedNames.emplace_back(script->GetName());
		}

		void CollectUnloaded(LuaManager::UnloadedScript& script)
		{
			g_UnloadedScripts.emplace_back(script.m_Name, script.m_Path);
		}

		std::vector<std::string> ScriptRows()
		{
			g_LoadedNames.clear();
			g_UnloadedScripts.clear();
			LuaManager::ForAllLoadedScripts(CollectLoadedName);
			LuaManager::ForAllUnloadedScripts(CollectUnloaded);

			auto rows = g_LoadedNames;
			for (auto& [name, path] : g_UnloadedScripts)
				rows.push_back(std::format("{}{}", kUnloadedPrefix, name));
			return rows;
		}

		// Looks up a loaded script by name - same captureless-callback
		// dance as above.
		std::string g_LookupTarget;
		std::shared_ptr<LuaScript> g_LookupResult;

		void CheckNameMatch(std::shared_ptr<LuaScript>& script)
		{
			if (script->GetName() == g_LookupTarget)
				g_LookupResult = script;
		}

		std::shared_ptr<LuaScript> FindLoadedScriptByName(const std::string& name)
		{
			g_LookupTarget = name;
			g_LookupResult.reset();
			LuaManager::ForAllLoadedScripts(CheckNameMatch);
			return g_LookupResult;
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
	LuaScriptsGrid::LuaScriptsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void LuaScriptsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Scripts - loaded scripts by name, unloaded ones prefixed (see
		// this class's own header comment for why); clicking an
		// unloaded row loads it, clicking a loaded one selects it for
		// the Editor section below.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Scripts", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    ScriptRows,
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    if (value.starts_with(kUnloadedPrefix))
			    {
				    auto name = value.substr(kUnloadedPrefix.size());
				    for (auto& [scriptName, path] : g_UnloadedScripts)
				    {
					    if (scriptName == name)
					    {
						    LuaManager::LoadScript(path);
						    break;
					    }
				    }
				    return;
			    }

			    m_SelectedScript = FindLoadedScriptByName(value);
		    }));

		// Editor (RenderSavedPlayers-style editor pane, here for
		// whichever loaded script is selected) - every row gated on
		// IsSelectionValid()-equivalent predicates, see this class's own
		// header comment for why that also covers the original's own
		// per-frame "did it unload underneath us" guard.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Editor", Theme::kText));

		auto isSelectionValid = [this] {
			return m_SelectedScript && m_SelectedScript->GetLoadState() != LuaScript::LoadState::UNLOADED;
		};

		// watchCondition() (not GridItemConditional) so a hidden row
		// below doesn't reserve its own layout slot - see
		// Grid::watchCondition()'s own doc comment.
		if (watchCondition(isSelectionValid))
		{
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    Theme::kContentWidth,
			    kItemH,
			    [this] {
				    return m_SelectedScript ? std::format("Selected: {}", m_SelectedScript->GetName()) : std::string();
			    },
			    Theme::kText));
		}

		// Pause/Resume are two separately-gated buttons rather than one
		// relabelled one - see this class's own header comment for why.
		if (watchCondition([this] {
			    return m_SelectedScript && m_SelectedScript->GetLoadState() == LuaScript::LoadState::RUNNING;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Pause", [this] {
				if (m_SelectedScript)
					m_SelectedScript->Pause();
			}));
		}
		if (watchCondition([this] {
			    return m_SelectedScript && m_SelectedScript->GetLoadState() == LuaScript::LoadState::PAUSED;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Resume", [this] {
				if (m_SelectedScript)
					m_SelectedScript->Resume();
			}));
		}
		if (watchCondition(isSelectionValid))
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Reload", [this] {
				if (m_SelectedScript)
					m_SelectedScript->Reload();
			}));
		}
		if (watchCondition(isSelectionValid))
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Unload", [this] {
				if (m_SelectedScript)
					m_SelectedScript->Unload();
			}));
		}
	}
}
