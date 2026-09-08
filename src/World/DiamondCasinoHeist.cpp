#include "World/DiamondCasinoHeist.hpp"
#include "Menu/Items.hpp"
#include <memory>

namespace Stand::Submenus
{
	std::shared_ptr<TabItem> RenderDiamondCasinoHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Diamond Casino Heist");

		auto autocuts = std::make_shared<Group>("Auto Cuts", 1);
		auto cuts = std::make_shared<Group>("Heist Cuts", 2);
		auto setups = std::make_shared<Group>("Heist Setups");
		auto loots = std::make_shared<Group>("Loots", 2);
		auto hacks = std::make_shared<Group>("Hacking", 2);
		auto misc = std::make_shared<Group>("Misc", 1);
		auto teleport = std::make_shared<Group>("Teleport");

		autocuts->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistplayers"_J));
		autocuts->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsetmaxpayout"_J));

		cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut1"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut3"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut2"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut4"_J));
		cuts->AddItem(std::make_shared<CommandItem>("diamondcasinoheistforceready"_J));
		cuts->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsetcuts"_J));

		auto gunmanAndDriver = std::make_shared<Group>("", 2);
		gunmanAndDriver->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistgunman"_J));
		gunmanAndDriver->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistdriver"_J));
		gunmanAndDriver->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistweapon"_J));
		gunmanAndDriver->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistvehicle"_J));

		setups->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistdifficulty"_J));
		setups->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistprimarytarget"_J));
		setups->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistapproach"_J));
		setups->AddItem(std::move(gunmanAndDriver));
		setups->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheisthacker"_J));
		setups->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsetup"_J));

		loots->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistpotentialtake"_J));
		loots->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistactualtake"_J));
		loots->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsetpotentialtake"_J, "Set##potentialtake"));
		loots->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsetactualtake"_J, "Set##actualtake"));

		hacks->AddItem(std::make_shared<CommandItem>("diamondcasinoheistskiphacking"_J));
		hacks->AddItem(std::make_shared<CommandItem>("diamondcasinoheistskipdrilling"_J));
		hacks->AddItem(std::make_shared<CommandItem>("diamondcasinoheistsolomantrap"_J));
		hacks->AddItem(std::make_shared<CommandItem>("diamondcasinoheistremovecameras"_J));
		hacks->AddItem(std::make_shared<CommandItem>("diamondcasinoheistkeycard"_J));
		hacks->AddItem(std::make_shared<BoolCommandItem>("autocollecttargets"_J));

		misc->AddItem(std::make_shared<CommandItem>("diamondcasinoheistinstantfinish"_J));

		teleport->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistteleportlist"_J));
		// teleport->AddItem(std::make_shared<BoolCommandItem>("diamondcasinoheistotherteleport"_J));
		// teleport->AddItem(std::make_shared<ConditionalItem>("diamondcasinoheistotherteleport"_J, std::make_shared<ListCommandItem>("diamondcasinoheistotherteleportlist"_J)));
		teleport->AddItem(std::make_shared<CommandItem>("diamondcasinoheistteleport"_J));

		tab->AddItem(autocuts);
		tab->AddItem(cuts);
		tab->AddItem(setups);
		tab->AddItem(loots);
		tab->AddItem(hacks);
		tab->AddItem(misc);
		tab->AddItem(teleport);

		return tab;
	}
}