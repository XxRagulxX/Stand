#pragma once
#include "Rendering/GridItem.hpp"

#include <string>

namespace Stand::Rendering
{
	// A label with an on/off indicator, mirroring how Stand's own list
	// items show bool-command state inline. Clicking flips its own local
	// state - not yet wired to a real CommandToggle (that's follow-up work
	// once a real menu page uses this instead of TestGrid).
	class GridItemToggle : public GridItem
	{
	public:
		GridItemToggle(int16_t width, int16_t height, std::string label, bool state) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Label(std::move(label)),
		    m_State(state)
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

		bool GetState() const
		{
			return m_State;
		}

	private:
		std::string m_Label;
		bool m_State;
	};
}
