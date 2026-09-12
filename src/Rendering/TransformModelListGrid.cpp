#include "Rendering/TransformModelListGrid.hpp"

#include "Commands/Self/CommandTransformModel.hpp"
#include "Rendering/GridItemTransformModel.hpp"
#include "Rendering/Theme.hpp"

#include <unordered_map>

namespace Stand::Rendering
{
	TransformModelListGrid::TransformModelListGrid(Stand::CommandList* list) :
	    Grid(Theme::GetContentOrigin(), 0),
	    m_List(list)
	{
	}

	TransformModelListGrid& TransformModelListGrid::GetOrCreate(Stand::CommandList* list)
	{
		static std::unordered_map<Stand::CommandList*, TransformModelListGrid> grids;
		return grids.try_emplace(list, list).first->second;
	}

	void TransformModelListGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		if (!m_List)
			return;

		for (auto& child : m_List->children)
		{
			auto* model = static_cast<Stand::CommandTransformModel*>(child.get());
			items_draft.push_back(std::make_unique<GridItemTransformModel>(Theme::kContentWidth, Theme::kContentItemHeight, model));
		}
	}
}
