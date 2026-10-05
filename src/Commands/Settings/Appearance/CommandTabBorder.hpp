#pragma once
#include "Rendering/StandPort/CommandColour.hpp"
#include "Commands/Settings/Appearance/CommandTabColours.hpp"
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

	class CommandBorderColour : public CommandColour
	{
	public:
		explicit CommandBorderColour(CommandList* const parent)
			: CommandColour(parent, LIT("Colour"), CMDNAMES("border"),
				LIT("The colour of the item border."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kBorderColour = {rgba.x, rgba.y, rgba.z, rgba.w};
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
		CommandColour* const colour;
		CommandBorderRainbow* const rainbow;

		explicit CommandTabBorder()
			: CommandList(nullptr, LIT("Border"))
			, width(createChild<CommandBorderWidth>())
			, rounded(createChild<CommandBorderRounded>())
			, colour(createChild<CommandBorderColour>())
			, rainbow(createChild<CommandBorderRainbow>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabBorder& GetCommandTabBorder();
}
