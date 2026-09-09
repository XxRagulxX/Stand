#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandSliderFloat : public CommandSlider
	{
	public:
		uint8_t precision = 2;

		explicit CommandSliderFloat(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, int min_value, int max_value, int default_value, unsigned int step_size = 1, commandflags_t flags = CMDFLAGS_SLIDER, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}) :
		    CommandSlider(parent, std::move(menu_name), std::move(command_names), std::move(help_text), min_value, max_value, default_value, step_size, flags, perm, default_hotkeys, COMMAND_SLIDER_FLOAT)
		{
		}

		[[nodiscard]] int getPrecisionScalar() const noexcept
		{
			int scalar = 1;
			for (uint8_t i = 0; i < precision; ++i)
				scalar *= 10;
			return scalar;
		}

		[[nodiscard]] float getFloatValue() const override
		{
			return static_cast<float>(value) / static_cast<float>(getPrecisionScalar());
		}

		[[nodiscard]] std::string getValueText() const override;

		void setValue(float value, Click& click);
	};
}
