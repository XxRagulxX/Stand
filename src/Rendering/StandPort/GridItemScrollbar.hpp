#pragma once
#include "Rendering/GridItem.hpp"

namespace Stand::Rendering
{
	class Grid;

	class GridItemScrollbar : public GridItem
	{
	public:
		explicit GridItemScrollbar(Grid* view = nullptr) :
		    GridItem(GRIDITEM_SCROLLBAR, 0, 0, 0, ALIGN_TOP_RIGHT),
		    m_View(view)
		{
		}

		void draw() override;

		void SetView(Grid* view)
		{
			m_View = view;
		}

	private:
		Grid* m_View;
	};
}
