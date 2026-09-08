#include "Menu/Menu.hpp"
#include "Menu/UIManager.hpp"
#include "Menu/MenuSelf.hpp"
#include "Menu/MenuVehicle.hpp"
#include "Menu/MenuTeleport.hpp"
#include "Menu/MenuNetwork.hpp"
#include "Menu/MenuPlayers.hpp"
#include "Menu/MenuRecovery.hpp"
#include "Menu/MenuSettings.hpp"
#include "Menu/MenuDebug.hpp"
#include "Menu/MenuWorld.hpp"

namespace Stand
{
	// Nothing draws this tree at all any more - see Menu/UIManager.hpp's
	// own class comment for why it still gets built regardless. This
	// file's only job is building it once, at startup.
	void Menu::Init()
	{
		// Arguably the only place this file should be edited at for more menus
		UIManager::AddSubmenu(std::make_shared<Submenus::Self>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Vehicle>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Teleport>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Network>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Players>());
		UIManager::AddSubmenu(std::make_shared<Submenus::World>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Recovery>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Settings>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Debug>());
	}
}
