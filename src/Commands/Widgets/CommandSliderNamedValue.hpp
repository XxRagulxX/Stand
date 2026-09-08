#pragma once
#include "Commands/CommandSliderLegacy.hpp"

#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandSliderNamedValue - a slider
	// where one specific value (usually its own floor) displays a name
	// ("Don't Override", real Stand's own Maximum Health/Respawn Delay
	// both use this) instead of the raw number. CommandSlider has no
	// display-formatting hook at all (GridItemCommandSlider::drawText()
	// hardcodes std::to_string()) - rather than add one there (which
	// would mean touching an existing file this pass is meant to leave
	// alone - see this folder's own top-level intent), GetDisplayText()
	// below is a new public method read by a dedicated new widget,
	// GridItemCommandSliderNamedValue (Rendering/), instead.
	class CommandSliderNamedValue : public CommandSliderLegacy
	{
	public:
		CommandSliderNamedValue(std::string name,
		    std::string label,
		    std::string description,
		    int min,
		    int max,
		    int def_val,
		    int namedValue,
		    std::string nameForValue) :
		    CommandSliderLegacy(std::move(name), std::move(label), std::move(description), min, max, def_val),
		    m_NamedValue(namedValue),
		    m_NameForValue(std::move(nameForValue))
		{
		}

		std::string GetDisplayText()
		{
			if (GetState() == m_NamedValue)
				return m_NameForValue;

			return std::to_string(GetState());
		}

	private:
		int m_NamedValue;
		std::string m_NameForValue;
	};
}
