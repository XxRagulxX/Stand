#pragma once
#include "Rendering/GridItem.hpp"

#include <string>

namespace Stand::Rendering
{
	class Grid;

	// A list row ending in a right-aligned ">" - Stand's own way of
	// nesting a category inside another's list instead of a tab/sidebar
	// entry (see e.g. "Spawner >", "Garage >" under its Vehicle submenu).
	// Clicking one drills into m_Target via MenuNavigation::Push();
	// there's no ownership here (m_Target is one of the content Grids
	// like SelfGrid/VehicleGrid already own as file-scope statics).
	class GridItemFolder : public GridItem
	{
	public:
		GridItemFolder(int16_t width, int16_t height, std::string label, Grid* target) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Label(std::move(label)),
		    m_Target(target)
		{
		}

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		void activate() override;

	private:
		std::string m_Label;
		Grid* m_Target;
	};
}
