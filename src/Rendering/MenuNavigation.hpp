#pragma once
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class Grid;

	// The drill-down navigation stack behind MenuGrid's content column -
	// mirrors how Stand's own menu actually works: clicking a nested
	// category (a row ending in ">") replaces the content list in place
	// with that category's own items, and Backspace pops back to the
	// previous one. This replaces the horizontal-tab-strip navigation
	// the very first cut of this system used, which doesn't match how
	// Stand's menu behaves at all (Stand has no tab strip - "Main"'s
	// items and the other categories are all just rows/nested rows in
	// the same list).
	//
	// A free-standing global (not a member of MenuGrid) on purpose:
	// GridItemFolder - built inside content Grids like SelfGrid/
	// VehicleGrid, which have no reference to MenuGrid itself - needs to
	// push onto this directly from onClick(). MenuGrid only reads it
	// (Current()/BreadcrumbPath()) and resets it when the sidebar
	// selection changes (Reset()).
	class MenuNavigation
	{
	public:
		struct Level
		{
			std::string Label;
			Grid* Content;
		};

		// Replaces the whole stack with a single root level. Called
		// whenever the sidebar selection changes, so switching submenus
		// always starts at that submenu's own root rather than staying
		// mid-drill-down from whatever the previously active submenu was
		// showing.
		static void Reset(std::string rootLabel, Grid* rootContent);

		// Drills into a nested category (a GridItemFolder click).
		static void Push(std::string label, Grid* content);

		// Backs out one level (a Backspace press) - a no-op if already
		// at the root, which always stays on the stack.
		static void Pop();

		// The Grid currently on top of the stack - what MenuGrid::draw()/
		// drawText()/findItemAt() should actually delegate to. nullptr
		// only before the first Reset() call.
		static Grid* Current();

		// Every level's label, " > "-joined (e.g. "Vehicle > Spawner") -
		// mirrors Stand's own address-bar breadcrumb. Doesn't include the
		// app name prefix ("StandEnhanced"/"Stand ..."); MenuGrid adds that
		// itself.
		static std::string BreadcrumbPath();

	private:
		static std::vector<Level> s_Stack;
	};
}
