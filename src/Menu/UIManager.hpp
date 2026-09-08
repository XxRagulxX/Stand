#pragma once
#include "Menu/Category.hpp"
#include "Menu/Submenu.hpp"

namespace Stand
{
	// The classic menu tree - Category/Submenu/UIManager/Items.hpp's own
	// *Item classes. This project has fully moved onto the Stand-style
	// Grid/DirectXTK12 menu (see Rendering/GridRenderer.hpp's own class
	// comment) as its only native, user-facing UI - nothing draws this
	// tree any more (Classic.cpp/Menu/ClassicUI.hpp, the DirectXTK
	// widget kit that used to render it once ImGui itself was removed,
	// were deleted outright rather than kept as an unreachable renderer).
	//
	// This whole class survives purely as a plain data model for Lua's
	// own menu-building API (Scripting/LuaMenu.cpp's add_category()/
	// add_group()/add_bool_command()/find_submenu()/... and the raw
	// cat:imgui()/group:imgui() escape hatch into Scripting/LuaImGui.cpp,
	// itself long since reduced to inert stubs - see that file's own
	// comment): existing scripts calling these still run and get real
	// handles back without erroring, they just no longer draw anything
	// anywhere. All nine of Self/Vehicle/Teleport/Network/Players/World/
	// Recovery/Settings/Debug (see each one's own MenuXxx.cpp) still get
	// created and added here from Menu::Init(), each with an empty
	// constructor, purely so a Lua script that attaches into an existing
	// native submenu by name (menu.find_submenu("Self"):add_category(...))
	// still has somewhere real to attach to. A real Lua-facing UI on top
	// of Grid is its own future, separate piece of work.
	class UIManager
	{
	public:
		static void AddSubmenu(const std::shared_ptr<Submenu>&& submenu)
		{
			GetInstance().AddSubmenuImpl(std::move(submenu));
		}

		static void RemoveSubmenu(const std::shared_ptr<Submenu>& submenu)
		{
			GetInstance().RemoveSubmenuImpl(submenu);
		}

		static void SetActiveSubmenu(const std::shared_ptr<Submenu> submenu)
		{
			GetInstance().SetActiveSubmenuImpl(submenu);
		}

		static std::shared_ptr<Submenu> GetActiveSubmenu()
		{
			return GetInstance().GetActiveSubmenuImpl();
		}

		static std::shared_ptr<Category> GetActiveCategory()
		{
			return GetInstance().GetActiveCategoryImpl();
		}

		static const std::vector<std::shared_ptr<Submenu>>& GetSubmenus()
		{
			return GetInstance().m_Submenus;
		}

	private:
		static inline UIManager& GetInstance()
		{
			static UIManager instance;
			return instance;
		}

		void AddSubmenuImpl(const std::shared_ptr<Submenu>&& submenu);
		void RemoveSubmenuImpl(const std::shared_ptr<Submenu>& submenu);
		void SetActiveSubmenuImpl(const std::shared_ptr<Submenu> submenu);
		std::shared_ptr<Submenu> GetActiveSubmenuImpl();
		std::shared_ptr<Category> GetActiveCategoryImpl();

		std::shared_ptr<Submenu> m_ActiveSubmenu;
		std::vector<std::shared_ptr<Submenu>> m_Submenus;
	};
}
