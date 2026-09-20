#pragma once
#include "Commands/Settings/CommandTabColours.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>

namespace Stand
{
	class CommandBorderWidth : public CommandSlider
	{
	public:
		explicit CommandBorderWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("borderwidth"),
				NOLABEL, 0, SHRT_MAX, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kBorderWidth = static_cast<int16_t>(value);
		}
	};

	class CommandBorderRounded : public CommandToggle
	{
	public:
		explicit CommandBorderRounded(CommandList* const parent)
			: CommandToggle(parent, LIT("Rounded"), CMDNAMES("borderrounded"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kBorderRounded = m_on;
		}
	};

	class CommandBorderRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandBorderRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("borderrainbow"),
				Rendering::Theme::kBorderColour)
		{
		}
	};

	class CommandTabBorder : public CommandList
	{
	public:
		CommandBorderWidth* const width;
		CommandBorderRounded* const rounded;
		CommandBorderRainbow* const rainbow;

		explicit CommandTabBorder()
			: CommandList(nullptr, LIT("Border"), CMDNAMES())
			, width(createChild<CommandBorderWidth>())
			, rounded(createChild<CommandBorderRounded>())
			, rainbow(createChild<CommandBorderRainbow>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabBorder& GetCommandTabBorder();
}
