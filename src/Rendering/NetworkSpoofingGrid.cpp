#include "Rendering/NetworkSpoofingGrid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// mmregion's own condition is spoofmmregion being on AND
		// spoofmmregion's own row being visible in the first place
		// (cheaterpool being off) - two BoolCommands, not one, so this
		// needs GridItemConditional's std::function<bool()> overload
		// rather than its plain joaat_t one.
		bool ShouldShowMmRegion()
		{
			auto* cheaterpool = Commands::GetCommand<CommandToggleLegacy>("cheaterpool"_J);
			auto* spoofmmregion = Commands::GetCommand<CommandToggleLegacy>("spoofmmregion"_J);
			return (!cheaterpool || !cheaterpool->GetState()) && spoofmmregion && spoofmmregion->GetState();
		}
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	NetworkSpoofingGrid::NetworkSpoofingGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void NetworkSpoofingGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Matchmaking (Client) (matchmakingGroup) - cheaterpool and
		// spoofdatahash are unconditional. The spoofMMRegion subgroup
		// (spoofmmregion + its own further-conditional mmregion list) is
		// gated on cheaterpool being *off* (negate) in the original -
		// spoofmmregion below carries that same condition directly
		// (there's no single GridItem standing in for "the subgroup"
		// here to wrap once), and mmregion's own condition composes both
		// checks via ShouldShowMmRegion() above. watchCondition() (not
		// GridItemConditional) so a hidden row doesn't reserve its own
		// layout slot - see Grid::watchCondition()'s own doc comment.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Matchmaking (Client)", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "cheaterpool"_J));
		if (watchCondition("cheaterpool"_J, true))
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spoofmmregion"_J, "Spoof Region"));
		if (watchCondition(ShouldShowMmRegion))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "mmregion"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spoofdatahash"_J));

		// Matchmaking (Server) (matchmakingSrvGroup) - all four toggles
		// are unconditional; their paired mmregiontype/mmlanguage/
		// mmplayercount/mmmultiplexsessioncount items are each gated on
		// the toggle right next to it.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Matchmaking (Server)", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "mmspoofregiontype"_J));
		if (watchCondition("mmspoofregiontype"_J))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "mmregiontype"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "mmspooflanguage"_J));
		if (watchCondition("mmspooflanguage"_J))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "mmlanguage"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "mmspoofplayercount"_J));
		if (watchCondition("mmspoofplayercount"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "mmplayercount"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "mmmultiplexsession"_J));
		if (watchCondition("mmmultiplexsession"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "mmmultiplexsessioncount"_J));
	}
}
