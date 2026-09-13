#include "Rendering/SettingsGameGrid.hpp"

#include "Commands/ESP/CommandTabESP.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemStandCommand.hpp"
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

	SettingsGameGrid::SettingsGameGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGameGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& esp = Features::GetCommandTabESP();

		// Player ESP
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Player ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawPlayers));
		if (watchCondition([&esp] { return esp.drawPlayers->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawDeadPlayers));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.namePlayers));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "namecolorplayers"_J,
		    [&esp] { return esp.drawPlayers->m_on; });
		if (watchCondition([&esp] { return esp.drawPlayers->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distancePlayers));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.skeletonPlayers));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "skeletoncolorplayers"_J,
		    [&esp] { return esp.drawPlayers->m_on; });

		// Ped ESP
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Ped ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawPeds));
		if (watchCondition([&esp] { return esp.drawPeds->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawDeadPeds));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.modelPeds));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "hashcolorpeds"_J,
		    [&esp] { return esp.drawPeds->m_on; });
		if (watchCondition([&esp] { return esp.drawPeds->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.netInfoPeds));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.scriptInfoPeds));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distancePeds));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.skeletonPeds));
		}
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "skeletoncolorpeds"_J,
		    [&esp] { return esp.drawPeds->m_on; });

		// Object ESP
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Object ESP", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawObjects));
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "hashcolorobjects"_J,
		    [&esp] { return esp.drawObjects->m_on; });
		if (watchCondition([&esp] { return esp.drawObjects->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.netInfoObjects));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.scriptInfoObjects));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distanceObjects));
		}
	}
}
