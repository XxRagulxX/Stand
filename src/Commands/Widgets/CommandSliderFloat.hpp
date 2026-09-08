#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

// Stores its value as an int scaled by 10^precision (real Stand's own
// representation) - e.g. min=1/max=1000000/default=100/step=10 at the
// default precision (2) is really a 0.01-10000.00 range, default 1.00,
// step 0.10. getFloatValue()/setValue(float, Click&) convert to/from the
// real float; onLeft()/onRight()'s integer step logic (inherited from
// CommandSlider, unchanged) is what actually moves the scaled value.
namespace Stand
{
	class CommandSliderFloatLegacy : public CommandSlider
	{
	public:
		uint8_t precision = 2;

		explicit CommandSliderFloatLegacy(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, int min_value, int max_value, int default_value, unsigned int step_size = 1, commandflags_t flags = CMDFLAGS_SLIDER, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}) :
		    CommandSlider(parent, std::move(menu_name), std::move(command_names), std::move(help_text), min_value, max_value, default_value, step_size, flags, perm, default_hotkeys, COMMAND_SLIDER_FLOAT)
		{
		}

		[[nodiscard]] int getPrecisionScalar() const;
		[[nodiscard]] float getFloatValue() const override;

		void setValue(float value, Click& click);
	};
}
