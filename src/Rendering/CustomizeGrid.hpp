#pragma once
#include "Rendering/Grid.hpp"

#include <DirectXMath.h>
#include <string>

namespace Stand::Rendering
{
	// Content grid for Settings > Customize: replaces the classic menu's
	// own Customize page (Config/GUISettings.cpp's DrawGUISettingsMenu())
	// rather than porting it - that page edits ImGuiStyle/ImGuiCol_* (52
	// colours plus rounding/layout/border/global/font sliders), which is
	// meaningless once this Grid renderer is primary and doesn't draw
	// through ImGui at all.
	//
	// What this actually exposes instead: Theme.hpp's own six colours
	// (kAccent/kPanelBackground/kText/kToggleOff/kError/
	// kPlaceholderText) - the entire palette this renderer draws with -
	// each as four discrete R/G/B/A steppers (0-255), same "no gradient/
	// shader support, so every channel steps individually" tradeoff as
	// GridItemCommandColourCustom's own class comment. Theme.hpp's own six
	// colours are `inline` (mutable) rather than `constexpr` specifically
	// so this page can write straight into them - see Theme.hpp's own
	// comment on why that's safe everywhere else that reads them.
	//
	// Persisted via Theme::LoadFromDisk()/SaveToDisk() (grid_theme.json,
	// a new, GridRenderer-only file - the classic Customize page's own
	// themes.json stores ImGui-specific keys this has no use for).
	// LoadFromDisk() is called from populate(), not the constructor -
	// this Grid is a file-scope static (see SettingsGrid.cpp), so its
	// constructor runs during C++ static initialization, before main()
	// calls FileMgr::Init() - see GlobalsGrid.cpp's identical note.
	//
	// Settings > Appearance > Colours (SettingsColoursGrid, added
	// later - real Stand's own Primary/Background Colour, see Commands/
	// Settings/CommandPrimaryColour.cpp/CommandBackgroundColour.cpp)
	// edits these same two colours (Theme::kAccent/kPanelBackground)
	// through a different path - real Command objects, persisted via
	// the standard Command config rather than grid_theme.json. Editing
	// the same colour from both pages in the same session works (each
	// writes Theme::kAccent/kPanelBackground directly, so whichever was
	// touched last wins, and this page's own populate() rereads the
	// live value fresh every time it opens) but the two pages don't
	// know about each other's own separate storage - a real, disclosed
	// rough edge from having two theme-editing UIs rather than
	// unifying them, left alone since the user only asked to add the
	// new one, not retire this one.
	class CustomizeGrid : public Grid
	{
	public:
		CustomizeGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void AddColorRows(std::vector<std::unique_ptr<GridItem>>& items_draft, const std::string& label, DirectX::XMFLOAT4& color);
	};
}
