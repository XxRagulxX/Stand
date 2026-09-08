#pragma once
#include "Rendering/GridItem.hpp"

#include <functional>
#include <string>

namespace Stand::Rendering
{
	// A button-styled rectangle with a centred label. Runs an optional
	// action callback on click (in addition to the log line, which stays
	// so a click is always independently verifiable in the log even for
	// a silent/purely-visual action, or when no callback is given at all).
	class GridItemButton : public GridItem
	{
	public:
		GridItemButton(int16_t width, int16_t height, std::string label, std::function<void()> action = nullptr) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Label(std::move(label)),
		    m_Action(std::move(action))
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
		std::function<void()> m_Action;
	};
}
