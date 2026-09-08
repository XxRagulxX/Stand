#include "World/Missions.hpp"

namespace Stand::Submenus
{
	std::shared_ptr<TabItem> RenderMissionsMenu()
	{
        auto tab = std::make_shared<TabItem>("Missions");


        auto setups = std::make_shared<Group>("Mission Setups",2);
        auto lowrider = std::make_shared<Group>("Low Rider",2);
        auto casino = std::make_shared<Group>("Casino",2);
        auto yach = std::make_shared<Group>("Yach",2);
        auto papertrail = std::make_shared<Group>("Papertrail",2);
        auto dragwar = std::make_shared<Group>("Dragwars",2);
        auto mercenaries = std::make_shared<Group>("Mercenaries",2);
        auto cluckinbell = std::make_shared<Group>("Cluckin Bell",2);
        auto tuners = std::make_shared<Group>("Tuners",4);
        auto contract = std::make_shared<Group>("Contract",4);
        auto savageyard = std::make_shared<Group>("Savage Yard",2);


        lowrider->AddItem(std::make_shared<ListCommandItem>("missionlowrider"_J));
        lowrider->AddItem(std::make_shared<CommandItem>("missionlowridersetup"_J,"Setup##lrsetup"));

        casino->AddItem(std::make_shared<ListCommandItem>("missioncasinostory"_J));
        casino->AddItem(std::make_shared<CommandItem>("missioncasinostorysetup"_J,"Setup##cssetup"));

        yach->AddItem(std::make_shared<ListCommandItem>("missionsuperyacht"_J));
        yach->AddItem(std::make_shared<CommandItem>("missionsuperyachtsetup"_J,"Setup##ysetup"));

        papertrail->AddItem(std::make_shared<ListCommandItem>("missionoperationpapertrail"_J));
        papertrail->AddItem(std::make_shared<CommandItem>("missionoperationpapertrailsetup"_J,"Setup##papersetup"));

        dragwar->AddItem(std::make_shared<ListCommandItem>("missiondrugwars"_J));
        dragwar->AddItem(std::make_shared<CommandItem>("missiondrugwarsmissionsetup"_J,"Setup##dwsetup"));

        mercenaries->AddItem(std::make_shared<ListCommandItem>("missionsamercenaries"_J));
        mercenaries->AddItem(std::make_shared<CommandItem>("missionsamercenariesmissionsetup"_J,"Setup##smsetup"));

        cluckinbell->AddItem(std::make_shared<ListCommandItem>("missioncluckinbellfarmraid"_J));
        cluckinbell->AddItem(std::make_shared<CommandItem>("missioncluckinbellfarmraidsetup"_J,"Setup##cbsetup"));
        cluckinbell->AddItem(std::make_shared<CommandItem>("missionclukinbellfarmraidcooldown"_J,"Remove Cooldown##cbrmcooldown"));

        tuners->AddItem(std::make_shared<ListCommandItem>("missiontunersrobbery"_J));
        tuners->AddItem(std::make_shared<CommandItem>("missiontunerscompletepreps"_J,"Complete Preps##tsetuppreps"));
        tuners->AddItem(std::make_shared<CommandItem>("missiontunersresetpreps"_J,"Reset Preps##TResetPreps"));
        tuners->AddItem(std::make_shared<CommandItem>("missiontunersresetcontracts"_J,"Reset Contracts##tresetcontracts"));
        tuners->AddItem(std::make_shared<CommandItem>("missiontunersrobberysetup"_J,"Setup##tsetup"));

        contract->AddItem(std::make_shared<ListCommandItem>("missioncontract"_J));
        contract->AddItem(std::make_shared<CommandItem>("missioncontractmissionssetup"_J,"Setup##contractsetup"));
        contract->AddItem(std::make_shared<CommandItem>("missioncontractremovecooldown"_J,"Remove Cooldown##contactrmcooldown"));

        savageyard->AddItem(std::make_shared<ListCommandItem>("missionsavageyard"_J));
        savageyard->AddItem(std::make_shared<CommandItem>("missionsavageyardrobberysetup"_J,"Setup##syrsetup"));
        

        tab->AddItem(lowrider);
        tab->AddItem(casino);
        tab->AddItem(yach);
        tab->AddItem(papertrail);
        tab->AddItem(dragwar);
        tab->AddItem(mercenaries);
        tab->AddItem(cluckinbell);
        tab->AddItem(tuners);
        tab->AddItem(contract);
        tab->AddItem(savageyard);

        return tab;
    
    }
}