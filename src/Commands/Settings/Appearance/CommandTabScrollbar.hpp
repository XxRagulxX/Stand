#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <string>

namespace Stand
{
	class CommandScrollbarMode : public CommandSlider
	{
		static constexpr const char* kLabels[3] = {"Enabled", "Enabled For Big Lists", "Disabled"};

	public:
		explicit CommandScrollbarMode(CommandList* const parent)
			: CommandSlider(parent, LIT("Scrollbar"), CMDNAMES("scrollbar"),
				NOLABEL, 0, 2, 1, 1)
		{
		}

		std::string getValueText() const override
		{
			return kLabels[std::clamp(value, 0, 2)];
		}

		void onChange(Click& click, int prev_value) override
		{
			switch (value)
			{
			case 0:
				Rendering::Theme::kScrollbarMode = Rendering::Theme::ScrollbarMode::Enabled;
				break;
			case 2:
				Rendering::Theme::kScrollbarMode = Rendering::Theme::ScrollbarMode::Disabled;
				break;
			default:
				Rendering::Theme::kScrollbarMode = Rendering::Theme::ScrollbarMode::EnabledWhenNeeded;
				break;
			}
		}
	};

	class CommandScrollbarWidth : public CommandSlider
	{
	public:
		explicit CommandScrollbarWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("scrollbarwidth"),
				NOLABEL, 1, SHRT_MAX, 6, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kScrollbarWidthMutable = static_cast<int16_t>(value);
		}
	};

	class CommandTabScrollbar : public CommandList
	{
	public:
		CommandScrollbarMode* const mode;
		CommandScrollbarWidth* const width;

		explicit CommandTabScrollbar()
			: CommandList(nullptr, LIT("Scrollbar"), CMDNAMES())
			, mode(createChild<CommandScrollbarMode>())
			, width(createChild<CommandScrollbarWidth>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabScrollbar& GetCommandTabScrollbar();
}
