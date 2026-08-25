#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Rendering/WaterQuad.hpp"

namespace Stand
{
	class CommandWaterOpacityDifferential : public CommandSlider
	{
	private:
		std::vector<WaterOpacityData> og_opacity = {};

	public:
		explicit CommandWaterOpacityDifferential(CommandList* const parent);

		Label getActivationName() const final;

		void onChange(Click& click, int prev_value) final;
	};
}
