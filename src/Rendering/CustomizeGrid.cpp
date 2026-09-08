#include "Rendering/CustomizeGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin (1438, 587) matches every other content Grid's - see the
	// comment in MenuGrid.cpp's anonymous namespace for why (no shared
	// header for these yet). Spacer is 0, not 3: confirmed against real
	// Stand's own source (origin/stand-reference) that individual list
	// rows have zero gap between them - the 3-unit spacer real Stand
	// does use is only ever between distinct chrome pieces (addressbar/
	// tabs/list), never between rows within one list, which is what every
	// item in this Grid's own list actually is. Each item below specifies
	// its own width (Theme::kContentWidth) rather than the Grid itself,
	// matching Stand's real Grid - see Grid.hpp's class comment.
	CustomizeGrid::CustomizeGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void CustomizeGrid::AddColorRows(std::vector<std::unique_ptr<GridItem>>& items_draft, const std::string& label, DirectX::XMFLOAT4& color)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, label, Theme::kText));

		auto addChannel = [&items_draft](const char* channelLabel, float& channel) {
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    channelLabel,
			    static_cast<int>(channel * 255.f),
			    0,
			    255,
			    [&channel](int value) {
				    channel = static_cast<float>(value) / 255.f;
				    Theme::SaveToDisk();
			    }));
		};

		addChannel("Red", color.x);
		addChannel("Green", color.y);
		addChannel("Blue", color.z);
		addChannel("Alpha", color.w);
	}

	void CustomizeGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// See this class's own header comment for why this runs here,
		// not the constructor.
		Theme::LoadFromDisk();

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Reset to Defaults", [this] {
			Theme::kAccent = {1.f, 0.f, 1.f, 1.f};
			Theme::kPanelBackground = {0.f, 0.f, 0.f, 0.3019f};
			Theme::kText = {1.f, 1.f, 1.f, 1.f};
			Theme::kToggleOff = {0.35f, 0.35f, 0.35f, 1.f};
			Theme::kError = {0.6f, 0.2f, 0.2f, 1.f};
			Theme::kPlaceholderText = {0.7f, 0.7f, 0.7f, 1.f};
			Theme::SaveToDisk();
			invalidate();
		}));

		AddColorRows(items_draft, "Accent", Theme::kAccent);
		AddColorRows(items_draft, "Panel Background", Theme::kPanelBackground);
		AddColorRows(items_draft, "Text", Theme::kText);
		AddColorRows(items_draft, "Toggle Off", Theme::kToggleOff);
		AddColorRows(items_draft, "Error", Theme::kError);
		AddColorRows(items_draft, "Placeholder Text", Theme::kPlaceholderText);
	}
}
