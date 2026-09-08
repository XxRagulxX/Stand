#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/Grid.hpp"

#include <unordered_map>

namespace Stand::Rendering
{
	// A Rendering::Grid that renders one real Stand::CommandList's own
	// children as rows - the Grid-bridge half of the project's phased
	// migration to real Stand's own Command/CommandList/CommandToggle/
	// CommandSlider foundation (Commands/Widgets/ - see
	// GridItemStandCommand.hpp's own class comment for the fuller
	// context on why this bridge exists and what it does/doesn't cover
	// yet). populate() walks list->children once (matching every other
	// content Grid's own "populate() builds a fixed row set, once,
	// lazily" contract - see Grid.hpp's own class comment on why this is
	// one-shot rather than Stand's own repeatable update()), pushing one
	// GridItemStandCommand per child.
	class GridStandCommandList : public Grid
	{
	public:
		explicit GridStandCommandList(Stand::CommandList* list);

		// One persistent GridStandCommandList per Stand::CommandList
		// instance, cached by pointer in a static map - a folder-style
		// row's own m_Target contract (GridItemFolder.hpp's own class
		// comment: "there's no ownership here") needs a stable,
		// non-owning Grid* that outlives the row, and a
		// Stand::CommandList's own children are owned by
		// std::unique_ptr<Command> already (nothing else to reach for
		// as a stable owner) - same "static cache keyed by identity,
		// handed out as a raw pointer" shape GridItemCommandColourCustom.cpp's
		// own GetColorEditGrid() already established for the same
		// reason (a ColorCommand's own R/G/B/A sub-page).
		static GridStandCommandList& GetOrCreate(Stand::CommandList* list);

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		Stand::CommandList* m_List;
	};
}
