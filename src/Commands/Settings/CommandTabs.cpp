#include "Commands/CommandListSelect.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandTabs (Commands/Widgets/
	// CommandTabs.hpp on origin/stand-reference) - show/hide the whole
	// sidebar (Theme::kTabsVisible), triggering a full menu re-layout the
	// same way real Stand's own onChange() calls g_menu_grid.update().
	class CommandTabs : public CommandToggleLegacy
	{
	public:
		CommandTabs() :
		    CommandToggleLegacy("tabs", "Tabs", "Whether the menu's own sidebar/tab strip is shown at all.", true)
		{
		}

	protected:
		void OnEnable() override
		{
			Sync();
		}

		void OnDisable() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandToggleLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			Rendering::Theme::kTabsVisible = GetState();
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	static CommandTabs _Tabs{};

	// Ported from real Stand's own CommandTabsPos (Commands/Stand/
	// CommandTabsPos.hpp on origin/stand-reference) - which side of the
	// menu the sidebar/tab strip renders on (Theme::kTabsPosition).
	class CommandTabsPos : public CommandListSelect
	{
	public:
		CommandTabsPos() :
		    CommandListSelect("tabsposition",
		        "Position",
		        "Which side of the menu the sidebar/tab strip renders on.",
		        {{0, "Left"}, {1, "Right"}, {2, "Top"}, {3, "Bottom"}},
		        0)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

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
			case 1:
				Rendering::Theme::kTabsPosition = Rendering::Theme::TabsPosition::Right;
				break;
			case 2:
				Rendering::Theme::kTabsPosition = Rendering::Theme::TabsPosition::Top;
				break;
			case 3:
				Rendering::Theme::kTabsPosition = Rendering::Theme::TabsPosition::Bottom;
				break;
			default:
				Rendering::Theme::kTabsPosition = Rendering::Theme::TabsPosition::Left;
				break;
			}

			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	static CommandTabsPos _TabsPos{};
}
