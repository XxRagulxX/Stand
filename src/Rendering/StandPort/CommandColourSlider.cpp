#include "Rendering/StandPort/CommandColourSlider.hpp"

#include "Rendering/StandPort/CommandColourCustom.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	void CommandColourSlider::onChange(Click& click, int prev_value)
	{
		if (click.type != CLICK_BULK)
		{
			auto* const colour = static_cast<CommandColourCustom*>(this->parent);
			switch (value_type)
			{
			case RGB:
				colour->updateHSV();
				break;

			case HSV:
				colour->updateRGB();
				break;
			}
			colour->processChange(click);
		}
	}
}
