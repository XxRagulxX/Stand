#pragma once
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class Grid;

	// Pushes ONE folder-style row (a label + right-aligned ">", same
	// shape as GridItemFolder) into items_draft, bound to a real
	// Stand::CommandColourCustom looked up by joaat hash - the Grid
	// equivalent of ColorCommandItem (src/ColorCommandItem.cpp) for the
	// ImGui menu, minus its own ImGui::ColorPicker4 popup (a continuous
	// 2D/gradient picker): clicking through instead opens a sub-page of
	// four discrete R/G/B/A steppers (0-255 each), same "no popup, step
	// through it instead" trade-off GridItemCommandListSelect/
	// GridItemCommandSlider already make.
	//
	// The row's own trailing ">" is tinted to the command's current
	// colour instead of a separate swatch box next to the label - ported
	// from real Stand's own GridItemList::update() (origin/stand-
	// reference), which does the exact same thing for its own
	// CommandColourCustom rows: retints the row's trailing arrow sprite
	// with the command's live RGBA (falling back to the normal colour
	// only while focused, if contrast against the focus highlight would
	// be too low - see GridItemColorFolder's own comment in the .cpp for
	// the contrast check itself). This project previously drew a
	// separate coloured swatch rect + label row ahead of the four
	// channel steppers instead - replaced now that this matches real
	// Stand's own actual behaviour more closely, and is one row instead
	// of five.
	void AddColorCommandRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);

	// AddColorCommandRows() above with no gating parameter of its own -
	// unlike a single-GridItem widget, there's nothing to hand a
	// condition directly. Registers condition with grid.watchCondition()
	// (see its own doc comment in Grid.hpp) and only builds/pushes the
	// one row when it's true, rather than always building it and
	// wrapping it in GridItemConditional - so a hidden row doesn't
	// reserve its own layout space, and the caller's own Grid (not this
	// free function) is what needs repopulating live when condition
	// changes. Needed wherever a CommandColourCustom swatch is itself behind a
	// ConditionalItem in the original ImGui menu (Weapons > Custom
	// Weapons' paintguncolor, Settings > Game's ESP name/skeleton/hash
	// colour swatches, ...).
	void AddConditionalColorCommandRows(Grid& grid,
	    std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    joaat_t id,
	    std::function<bool()> condition,
	    std::optional<std::string> labelOverride = std::nullopt);
}
