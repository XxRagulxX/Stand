#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <commdlg.h>
#include <string>
#include <windows.h>

namespace Stand
{
	class CommandCommandBoxInputScale : public CommandSlider
	{
	public:
		explicit CommandCommandBoxInputScale(CommandList* const parent)
			: CommandSlider(parent, LIT("Scale"), CMDNAMES("commandboxinputscale"),
				NOLABEL, 1, 32767, static_cast<int>(Rendering::Theme::kTextScale * 10000.f), 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandBoxInputScale = value / 10000.f;
		}
	};

	class CommandCommandBoxInputXOffset : public CommandSlider
	{
	public:
		explicit CommandCommandBoxInputXOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("X Offset"), CMDNAMES("commandboxinputxoffset"),
				NOLABEL, -15, 15, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandBoxInputXOffset = static_cast<float>(value);
		}
	};

	class CommandCommandBoxInputYOffset : public CommandSlider
	{
	public:
		explicit CommandCommandBoxInputYOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("Y Offset"), CMDNAMES("commandboxinputyoffset"),
				NOLABEL, -10, 10, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandBoxInputYOffset = static_cast<float>(value);
		}
	};

	class CommandCommandBoxInput : public CommandList
	{
	public:
		CommandCommandBoxInputScale* const scale;
		CommandCommandBoxInputXOffset* const xOffset;
		CommandCommandBoxInputYOffset* const yOffset;

		explicit CommandCommandBoxInput(CommandList* const parent)
			: CommandList(parent, LIT("Command Box Input Text"), CMDNAMES("commandboxinput"))
			, scale(createChild<CommandCommandBoxInputScale>())
			, xOffset(createChild<CommandCommandBoxInputXOffset>())
			, yOffset(createChild<CommandCommandBoxInputYOffset>())
		{
		}
	};

	class CommandSmallTextScale : public CommandSlider
	{
	public:
		explicit CommandSmallTextScale(CommandList* const parent)
			: CommandSlider(parent, LIT("Scale"), CMDNAMES("smalltextscale"),
				NOLABEL, 1, 32767, static_cast<int>(Rendering::Theme::kSmallTextScale * 10000.f), 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kSmallTextScaleMutable = value / 10000.f;
		}
	};

	class CommandSmallTextXOffset : public CommandSlider
	{
	public:
		explicit CommandSmallTextXOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("X Offset"), CMDNAMES("smalltextxoffset"),
				NOLABEL, -15, 15, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kSmallTextXOffset = static_cast<float>(value);
		}
	};

	class CommandSmallTextYOffset : public CommandSlider
	{
	public:
		explicit CommandSmallTextYOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("Y Offset"), CMDNAMES("smalltextyoffset"),
				NOLABEL, -10, 10, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kSmallTextYOffset = static_cast<float>(value);
		}
	};

	class CommandSmallText : public CommandList
	{
	public:
		CommandSmallTextScale* const scale;
		CommandSmallTextXOffset* const xOffset;
		CommandSmallTextYOffset* const yOffset;

		explicit CommandSmallText(CommandList* const parent)
			: CommandList(parent, LIT("Small Text"), CMDNAMES("smalltext"))
			, scale(createChild<CommandSmallTextScale>())
			, xOffset(createChild<CommandSmallTextXOffset>())
			, yOffset(createChild<CommandSmallTextYOffset>())
		{
		}
	};

	class CommandShowTextBoundingBoxes : public CommandToggle
	{
	public:
		explicit CommandShowTextBoundingBoxes(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Text Bounding Boxes"), CMDNAMES("showtextboundingboxes"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowTextBoundingBoxes = m_on;
		}
	};

	class CommandSetFont : public CommandPhysical
	{
	public:
		explicit CommandSetFont(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Set Font"), CMDNAMES("setfont"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			char filePath[MAX_PATH] = {};
			OPENFILENAMEA ofn{};
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "SpriteFont Files\0*.spritefont\0All Files\0*.*\0";
			ofn.lpstrFile = filePath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			if (GetOpenFileNameA(&ofn))
			{
				Rendering::Theme::kFontPath = filePath;
				Rendering::Theme::kFontReloadPending = true;
			}
		}
	};

	class CommandReloadFont : public CommandPhysical
	{
	public:
		explicit CommandReloadFont(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Reload Font"), CMDNAMES("reloadfont"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Rendering::Theme::kFontReloadPending = true;
		}
	};

	class CommandTabFontText : public CommandList
	{
	public:
		CommandSetFont* const setFont;
		CommandReloadFont* const reloadFont;
		CommandCommandBoxInput* const commandBoxInput;
		CommandSmallText* const smallText;
		CommandShowTextBoundingBoxes* const showTextBoundingBoxes;

		explicit CommandTabFontText()
			: CommandList(nullptr, LIT("Font & Text"), CMDNAMES())
			, setFont(createChild<CommandSetFont>())
			, reloadFont(createChild<CommandReloadFont>())
			, commandBoxInput(createChild<CommandCommandBoxInput>())
			, smallText(createChild<CommandSmallText>())
			, showTextBoundingBoxes(createChild<CommandShowTextBoundingBoxes>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabFontText& GetCommandTabFontText();
}
