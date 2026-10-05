#include "Rendering/Stand/World.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Commands/World/CommandTabWorld.hpp"

namespace Stand::Rendering
{
	World::World() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void World::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
		auto& worldTab = Features::GetCommandTabWorld();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.places));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.inhabitants));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.atmosphere));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.editor));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.water));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.worldstate));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.enhancedopenworld));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.dedsec));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.geoguessr));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.aestheticlight));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.worldborder));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.blackout));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.trains));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.arwaypoint));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.argps));
	}
}
