#pragma once
#include "Rendering/GridItem.hpp"

#include <functional>
#include <string>

namespace Stand::Rendering
{
	// Same on/off row GridItemCommandToggle draws, bound to an arbitrary
	// live bool via getter/setter callbacks instead of a real
	// Stand::CommandToggle looked up by joaat hash - for state that
	// isn't a registered Command at all (a plain struct field on some
	// other live object, e.g. SavedPlayerData::m_TrackPlayer), which
	// GridItemCommandToggle has no way to reach. Reads the getter every
	// frame (not tracking its own state) and calls the setter on click,
	// same live-state contract as GridItemCommandToggle.
	class GridItemBoundToggle : public GridItem
	{
	public:
		GridItemBoundToggle(int16_t width, int16_t height, std::string label, std::function<bool()> getter, std::function<void(bool)> setter);

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
		std::function<bool()> m_Getter;
		std::function<void(bool)> m_Setter;
	};
}
