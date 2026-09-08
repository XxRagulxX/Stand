#include "World/KortzCenterHeist.hpp"

namespace Stand::Submenus
{
	std::shared_ptr<TabItem> RenderKortzCenterHeistMenu()
	{
		auto tab   = std::make_shared<TabItem>("Kortz Center Heist");

		auto setups   = std::make_shared<Group>("Heist Setups");
		auto hacks = std::make_shared<Group>("Hacking", 2);
		auto misc = std::make_shared<Group>("Misc", 1);

		auto action    = std::make_shared<Group>("", 1);

		setups->AddItem(std::make_shared<ListCommandItem>("kortzcenterheistprimarytarget"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardroutes"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistglasscutter"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistpowerdrills"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistempcharges"_J));

		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistcaracara"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistannihilator"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistmanchez"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistcaracaraprep"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistannihilatorprep"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistmanchezprep"_J));

		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisthazmat"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheiststaffkeycard"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisttacticalequip"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisthackingdevice"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistaccesscode"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistunmarkedweapons"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardshipments"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistprepemp"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardroutesprep"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistglasscutterprep"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistpowerdrillsprep"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistempchargesprep"_J));

		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopeout"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistalphamail"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopesecondary"_J));
		setups->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopepoi"_J));

		setups->AddItem(std::make_shared<CommandItem>("kortzcenterheistsetup"_J));

		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistskipfingerprint"_J));
		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistskipsignalnodes"_J));
		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistskipdatacrack"_J));
		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistautoenterpcaccesscode"_J));
		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistcutglass"_J));
		hacks->AddItem(std::make_shared<CommandItem>("kortzcenterheistdisablelaser"_J));

		misc->AddItem(std::make_shared<CommandItem>("kortzcenterheisttakeprimary"_J));
		misc->AddItem(std::make_shared<CommandItem>("kortzcenterheisttakesecondary"_J));

		tab->AddItem(setups);
		tab->AddItem(hacks);
		tab->AddItem(misc);

		return tab;
	}
}