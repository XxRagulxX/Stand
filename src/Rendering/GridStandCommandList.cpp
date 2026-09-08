#include "Rendering/GridStandCommandList.hpp"

#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	// Origin/spacer match every other content Grid's.
	GridStandCommandList::GridStandCommandList(Stand::CommandList* list) :
	    Grid(Theme::GetContentOrigin(), 0),
	    m_List(list)
	{
	}

	GridStandCommandList& GridStandCommandList::GetOrCreate(Stand::CommandList* list)
	{
		static std::unordered_map<Stand::CommandList*, GridStandCommandList> grids;
		// try_emplace constructs the GridStandCommandList in place from
		// the forwarded list pointer (only if not already present)
		// rather than constructing a temporary and moving/copying it in -
		// Grid has no need to support either. Same shape
		// GridItemCommandColourCustom.cpp's own GetColorEditGrid() uses.
		return grids.try_emplace(list, list).first->second;
	}

	void GridStandCommandList::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		if (!m_List)
			return;

		for (auto& child : m_List->children)
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, Theme::kContentItemHeight, child.get()));
		}
	}
}
