#include "Commands/Widgets/CommandSliderFloat.hpp"

#include <iomanip>
#include <sstream>

namespace Stand
{
	std::string CommandSliderFloat::getValueText() const
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(precision) << getFloatValue();
		return oss.str();
	}

	void CommandSliderFloat::setValue(float value, Click& click)
	{
		const auto scalar = getPrecisionScalar();
		CommandSlider::setValue(click, static_cast<int>(value * static_cast<float>(scalar)));
	}
}
