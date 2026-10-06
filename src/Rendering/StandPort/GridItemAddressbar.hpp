#pragma once
#include "Rendering/GridItem.hpp"

#include <string>

namespace Stand::Rendering
{
	class GridItemAddressbar : public GridItem
	{
	public:
		GridItemAddressbar(int16_t width, int16_t height, std::string title) :
		    GridItem(GRIDITEM_PRIMARYTEXT, width, height),
		    m_Title(std::move(title))
		{
		}

		void draw() override;
		void drawText() override;

		void SetTitle(std::string title)
		{
			m_Title = std::move(title);
		}

	private:
		std::string m_Title;
	};
}
