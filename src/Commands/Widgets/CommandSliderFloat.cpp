#include "Commands/Widgets/CommandSliderFloat.hpp"

namespace Stand
{
	int CommandSliderFloatLegacy::getPrecisionScalar() const
	{
		int scalar = 1;
		for (uint8_t i = 0; i < precision; ++i)
			scalar *= 10;
		return scalar;
	}

	float CommandSliderFloatLegacy::getFloatValue() const
	{
		return static_cast<float>(value) / static_cast<float>(getPrecisionScalar());
	}

	void CommandSliderFloatLegacy::setValue(float value, Click& click)
	{
		const auto scalar = getPrecisionScalar();
		CommandSlider::setValue(click, static_cast<int>(value * static_cast<float>(scalar)));
	}
}
