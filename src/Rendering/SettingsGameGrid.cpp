#include "Rendering/SettingsGameGrid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Each group's own colour-swatch rows repeat the same
		// espdraw*-is-on condition their plain toggles already use -
		// named here once each rather than re-typing the same lambda at
		// every AddConditionalColorCommandRows() call site below.
		bool IsPlayerEspOn()
		{
			auto* espdrawplayers = Commands::GetCommand<CommandToggleLegacy>("espdrawplayers"_J);
			return espdrawplayers && espdrawplayers->GetState();
		}

		bool IsPedEspOn()
		{
			auto* espdrawpeds = Commands::GetCommand<CommandToggleLegacy>("espdrawpeds"_J);
			return espdrawpeds && espdrawpeds->GetState();
		}

		bool IsObjectEspOn()
		{
			auto* espdrawobjects = Commands::GetCommand<CommandToggleLegacy>("espdrawobjects"_J);
			return espdrawobjects && espdrawobjects->GetState();
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
	SettingsGameGrid::SettingsGameGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGameGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Player ESP (playerEsp) - every row below espdrawplayers is
		// gated on it (watchCondition(), not GridItemConditional, so
		// hidden rows don't reserve their own layout slot - see
		// Grid::watchCondition()'s own doc comment); namecolorplayers/
		// skeletoncolorplayers are CommandColourCustom swatches via
		// AddConditionalColorCommandRows, which does its own
		// watchCondition() call internally.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Player ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdrawplayers"_J));
		if (watchCondition("espdrawplayers"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdrawdeadplayers"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espnameplayers"_J, "Player Name"));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "namecolorplayers"_J, IsPlayerEspOn);
		if (watchCondition("espdrawplayers"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdistanceplayers"_J, "Player Distance"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espskeletonplayers"_J, "Player Skeleton"));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "skeletoncolorplayers"_J, IsPlayerEspOn);

		// Ped ESP (pedEsp) - same shape as Player ESP above, gated on
		// espdrawpeds instead.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Ped ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdrawpeds"_J));
		if (watchCondition("espdrawpeds"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdrawdeadpeds"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espmodelspeds"_J, "Ped Hashes"));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "hashcolorpeds"_J, IsPedEspOn);
		if (watchCondition("espdrawpeds"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espnetinfopeds"_J, "Ped Net Info"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espscriptinfopeds"_J, "Ped Script Info"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdistancepeds"_J, "Ped Distance"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espskeletonpeds"_J, "Ped Skeleton"));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "skeletoncolorpeds"_J, IsPedEspOn);

		// Object ESP (objectEsp) - gated on espdrawobjects.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Object ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdrawobjects"_J));
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "hashcolorobjects"_J, IsObjectEspOn);
		if (watchCondition("espdrawobjects"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espnetinfoobjects"_J, "Object Net Info"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espscriptinfoobjects"_J, "Object Script Info"));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "espdistanceobjects"_J, "Object Distance"));
		}
	}
}
