#include "Rendering/GridStandCommandList.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridItemText.hpp"
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

		if (m_List->requiresVehicle())
		{
			auto* list = m_List;
			bool in_veh = watchCondition([list] { return list->vehicleRequiredMessage() == nullptr; });
			if (!in_veh)
			{
				items_draft.push_back(std::make_unique<GridItemText>(
					Theme::kContentWidth, Theme::kContentItemHeight,
					list->vehicleRequiredMessage(), Theme::kText));
				return;
			}
		}

		{
			const size_t snap = m_List->countVisibleChildren();
			auto* list_for_watch = m_List;
			watchCondition([list_for_watch, snap] { return list_for_watch->countVisibleChildren() == snap; });
		}

		items_draft.reserve(m_List->children.size());
		for (auto& child : m_List->children)
		{
			if (child->isConcealed()) continue;
			if (child->isPhysical()) {
				const size_t before = items_draft.size();
				child->as<Stand::CommandPhysical>()->fillVirtualItems(items_draft, this);
				if (items_draft.size() != before) continue;
			}
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, Theme::kContentItemHeight, child.get()));
		}
	}
}
