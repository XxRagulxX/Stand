#include "Rendering/TransformGrid.hpp"

#include "Commands/Self/CommandTabTransform.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TransformModelListGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	TransformGrid::TransformGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void TransformGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabTransform();

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.findModel));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Playable Characters", &TransformModelListGrid::GetOrCreate(tab.playable)));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Ground Animals", &TransformModelListGrid::GetOrCreate(tab.groundAnimals)));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Water Animals", &TransformModelListGrid::GetOrCreate(tab.waterAnimals)));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Flying Animals", &TransformModelListGrid::GetOrCreate(tab.flyingAnimals)));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "NPCs", &TransformModelListGrid::GetOrCreate(tab.npcs)));
	}
}
