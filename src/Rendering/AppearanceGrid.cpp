#include "Rendering/AppearanceGrid.hpp"

#include "Commands/Self/CommandTabSelf.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TransformGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		TransformGrid g_TransformContent{};
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	AppearanceGrid::AppearanceGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void AppearanceGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Transform", &g_TransformContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Outfit", &GridStandCommandList::GetOrCreate(Features::GetCommandTabSelf().outfit)));
		for (auto& child : Features::GetCommandTabSelf().character->children)
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, child.get()));
	}
}
