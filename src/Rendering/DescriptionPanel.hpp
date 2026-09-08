#pragma once
#include <cstdint>

namespace Stand::Rendering
{
	// Shows the currently keyboard-focused content item's own
	// GetDescription() (help text), if it has one - this project's
	// equivalent of real Stand's own CommandPhysical::populateCorner()
	// (confirmed against origin/stand-reference), simplified to just
	// the one line every focused command actually has here
	// (Stand::Command::GetDescription()/Stand::CommandPhysical::
	// help_text - see GridItem::GetDescription()'s own comment), not
	// the syntax/range/toggle-correlation lines real Stand's own corner
	// also shows - none of that machinery exists on either of this
	// project's own two command systems yet.
	//
	// A free-standing global (not a GridItem, not owned by any Grid),
	// same shape as MenuPopup/MenuCommandBox - drawn by GridRenderer
	// alongside those every frame the menu is open, reading focus fresh
	// via MenuFocus::GetFocusedItem(MenuNavigation::Current()) rather
	// than needing anything pushed to it, and drawing nothing at all
	// when nothing's focused or the focused item has no description.
	//
	// Ports real Stand's own DEFAULT position - "Left" (CommandHelpPos's
	// own ALIGN_TOP_LEFT, g_renderer.info_text_alignment's default) -
	// not "Below Tabs" (ALIGN_BOTTOM_RIGHT), which two earlier passes
	// here wrongly assumed was the one to match. Confirmed against
	// origin/stand-reference's own Menu/Grid.cpp setPositions():
	// ALIGN_TOP_LEFT means "grow left from the anchor's own x/y" -
	// `x -= (item->width + spacer_x)`, anchored to the content list's
	// own top-left corner (self.main_view, since only the BELOW_TABS/
	// RIGHT cases override force_alignment_to) - i.e. this box's own
	// RIGHT edge sits just left of content's own left edge, growing
	// further left by its own fixed width (Theme::kInfoWidth, real
	// Stand's own info_width=300 default - NOT command_width, which
	// only the ALIGN_BOTTOM_LEFT case ever uses, confirmed against that
	// same ternary), y starting at content's own top. setPositions()
	// then finds the sidebar/tabs column already occupying that same
	// region (its own x-range overlaps this box's, since 300 > the
	// sidebar's own ~112-wide column) and pushes y down below the
	// sidebar's own bottom edge instead (getOccupantNarrow's collision
	// check) - which is why real Stand's own reference screenshot this
	// was ported against shows the box appearing below the tab list,
	// spilling left past it over open game world, rather than at
	// content's own top. contentX/contentY (MenuNavigation::Current()'s
	// own origin, runtime-position-adjusted the same way
	// MenuGrid::GetHeaderBarRect()/GetSidebarRect() already are) and
	// sidebarBottomY (GetSidebarRect()'s own y+height+spacer, or a value
	// below contentY - e.g. -1 - if there's no sidebar to collide with,
	// Theme::kTabsVisible false) are passed in rather than this class
	// reaching into MenuGrid directly, the same reason GridRenderer
	// already reads MenuGrid's other rects itself (see
	// GetHeaderBarRect()'s own comment); this class resolves the actual
	// collision (y = whichever of the two is lower) itself.
	class DescriptionPanel
	{
	public:
		static void Draw(int16_t contentX, int16_t contentY, int16_t sidebarBottomY);
		static void DrawText(int16_t contentX, int16_t contentY, int16_t sidebarBottomY);
	};
}
