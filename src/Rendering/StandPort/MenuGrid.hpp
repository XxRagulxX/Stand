#pragma once
#include "Rendering/StandPort/dx_common.hpp"
#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/GridItemScrollbar.hpp"
#include "Rendering/StandPort/TextSettings.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <cstdint>

namespace Stand::Rendering
{
	class GridItemAddressbar;
	class GridItemTabsVertical;
	class GridItemTabsHorizontal;

	class MenuGrid : public Grid
	{
	public:
		bool number_sliders_rightbound_values = true;
		bool left_space_before_all_commands = false;
		bool leftbound_textures_toggles = false;
		bool leftbound_textures_nontoggles = false;
		std::wstring untrimmed_menu_name{};

		bool addressbar_width_affected_by_columns = true;
		bool addressbar_width_affected_by_scrollbar = true;
		TextSettings addressbar_text{ float(12.0 * TEXT_HEIGHT_PX * 2.0), -2.0f, 2.0f };

		template<typename F>
		void updateSetting(F&& func) { func(); }

		MenuGrid();
		~MenuGrid() override;

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

		void HandleKey(unsigned int vkCode);
		void HandleMouseMove(int16_t hx, int16_t hy);
		void HandleMouseClick(int16_t hx, int16_t hy, bool ctrl, bool shift, bool doubleClick);
		void HandleMouseWheel(int16_t hx, int16_t hy, int delta);
		void InvalidateLayout()
		{
			invalidate();
		}

		bool GetHeaderBarRect(int16_t& x, int16_t& y, int16_t& width) const;
		bool GetSidebarRect(int16_t& x, int16_t& y, int16_t& width, int16_t& height) const;

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		[[nodiscard]] GridItem* SidebarItem() const;
		[[nodiscard]] size_t SidebarActiveIndex() const;
		void MoveSidebarActive(int delta);
		struct SubmenuRoot
		{
			size_t SidebarIndex;
			std::string Label; // breadcrumb root label, e.g. "Vehicle"
			Grid* Content;     // non-owning - see the class comment above
		};
		void SyncNavigation();

		GridItemAddressbar* m_Header = nullptr;
		GridItemTabsVertical* m_Sidebar = nullptr;
		GridItemTabsHorizontal* m_SidebarHorizontal = nullptr;
		GridItemScrollbar m_ContentScrollbar;
		std::vector<SubmenuRoot> m_Roots;
		size_t m_LastSidebarIndex = static_cast<size_t>(-1);
	};

	extern MenuGrid g_MenuGrid;
}

namespace Stand
{
	using Rendering::g_MenuGrid;
}
