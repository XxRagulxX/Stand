#pragma once

#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandFlags.hpp"

namespace Stand
{
	class CommandColourCustom;

	class CommandColourSlider : public CommandSlider
	{
	public:
		enum ValueType : uint8_t
		{
			RGB,
			HSV,
			OPACITY,
		};

	private:
		const ValueType value_type;

	public:
		explicit CommandColourSlider(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, const ValueType value_type, const int min_value, const int max_value, const int default_value)
			: CommandSlider(parent, std::move(menu_name), std::move(command_names), NOLABEL, min_value, max_value, default_value, 1, (CMDFLAGS_SLIDER & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS) | CMDFLAG_FEATURELIST_SKIP), value_type(value_type)
		{
		}

		void onChange(Click& click, int prev_value) final;
	};
}
