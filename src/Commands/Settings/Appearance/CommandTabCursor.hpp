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
	class CommandCursorPadding : public CommandSlider
	{
	public:
		explicit CommandCursorPadding(CommandList* const parent)
			: CommandSlider(parent, LIT("Scroll Gap"), CMDNAMES("cursorpadding"),
				LIT("Sets the minimum amount of commands you will see below or above your selection unless you're at the beginning or end of a list."),
				0, 25, 2, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCursorPadding = value;
		}
	};

	class CommandCursorBorderWidth : public CommandSlider
	{
	public:
		explicit CommandCursorBorderWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("cursorborderwidth"),
				NOLABEL, 0, SHRT_MAX, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCursorBorderWidth = static_cast<int16_t>(value);
		}
	};

	class CommandCursorBorderRounded : public CommandToggle
	{
	public:
		explicit CommandCursorBorderRounded(CommandList* const parent)
			: CommandToggle(parent, LIT("Rounded"), CMDNAMES("cursorborderrounded"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kCursorBorderRounded = m_on;
		}
	};

	class CommandCursorBorderRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandCursorBorderRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("cursorborderrainbow"),
				Rendering::Theme::kCursorBorderColour)
		{
		}
	};

	class CommandTabCursor : public CommandList
	{
	public:
		CommandCursorPadding* const padding;
		CommandCursorBorderWidth* const borderWidth;
		CommandCursorBorderRounded* const borderRounded;
		CommandCursorBorderRainbow* const borderRainbow;

		explicit CommandTabCursor()
			: CommandList(nullptr, LIT("Cursor"), CMDNAMES())
			, padding(createChild<CommandCursorPadding>())
			, borderWidth(createChild<CommandCursorBorderWidth>())
			, borderRounded(createChild<CommandCursorBorderRounded>())
			, borderRainbow(createChild<CommandCursorBorderRainbow>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabCursor& GetCommandTabCursor();
}
