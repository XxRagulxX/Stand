#pragma once
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class Grid;

	// Pushes a label + "Current" row followed by X/Y/Z stepper rows into
	// items_draft, all bound to the same real Stand::CommandPosition3d,
	// looked up by joaat hash - the Grid equivalent of Vector3CommandItem
	// (src/Vector3CommandItem.cpp) for the ImGui menu, minus its own
	// "Saved..." popup (a categorized saved-locations picker with
	// search) - this project has no popup/floating-overlay primitive
	// yet, so v1 here is InputFloat3 + "Current" only. "Current" only
	// does anything while Self::GetPed() is valid - registered with
	// grid.watchCondition() on that predicate and only pushed when true,
	// same as the original hiding the button entirely otherwise, but
	// without reserving its row's layout space while hidden (see
	// Grid::watchCondition()'s own doc comment).
	//
	// Four separate GridItem rows, not one, for the same reason
	// AddColorCommandRows() is - see GridItemCommandColourCustom.hpp's class
	// comment (every row here needs to plug into MenuFocus/
	// Grid::getFocusableItems() individually, which one mega-widget
	// covering all three axes couldn't do without its own separate
	// sub-focus concept).
	void AddVector3CommandRows(Grid& grid, std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);
}
