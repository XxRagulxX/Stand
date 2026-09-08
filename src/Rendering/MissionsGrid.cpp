#include "Rendering/MissionsGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin sits right below where RecoveryHeistsGrid's own tab strip
	// draws (587 + 32-tall strip + 3 spacer) - see this class's own
	// header comment.
	MissionsGrid::MissionsGrid() :
	    Grid(Theme::GetTabbedContentOrigin(), 0)
	{
	}

	void MissionsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Low Rider (lowrider)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Low Rider", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missionlowrider"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionlowridersetup"_J, "Setup"));

		// Casino (casino)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Casino", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missioncasinostory"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missioncasinostorysetup"_J, "Setup"));

		// Yach (yach) - kept verbatim, see this file's own header comment.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Yach", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missionsuperyacht"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionsuperyachtsetup"_J, "Setup"));

		// Papertrail (papertrail)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Papertrail", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missionoperationpapertrail"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionoperationpapertrailsetup"_J, "Setup"));

		// Dragwars (dragwar)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Dragwars", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missiondrugwars"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missiondrugwarsmissionsetup"_J, "Setup"));

		// Mercenaries (mercenaries)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Mercenaries", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missionsamercenaries"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionsamercenariesmissionsetup"_J, "Setup"));

		// Cluckin Bell (cluckinbell)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Cluckin Bell", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missioncluckinbellfarmraid"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missioncluckinbellfarmraidsetup"_J, "Setup"));
		items_draft.push_back(
		    std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionclukinbellfarmraidcooldown"_J, "Remove Cooldown"));

		// Tuners (tuners)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Tuners", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missiontunersrobbery"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missiontunerscompletepreps"_J, "Complete Preps"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missiontunersresetpreps"_J, "Reset Preps"));
		items_draft.push_back(
		    std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missiontunersresetcontracts"_J, "Reset Contracts"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missiontunersrobberysetup"_J, "Setup"));

		// Contract (contract)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Contract", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missioncontract"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missioncontractmissionssetup"_J, "Setup"));
		items_draft.push_back(
		    std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missioncontractremovecooldown"_J, "Remove Cooldown"));

		// Savage Yard (savageyard)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Savage Yard", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "missionsavageyard"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "missionsavageyardrobberysetup"_J, "Setup"));
	}
}
