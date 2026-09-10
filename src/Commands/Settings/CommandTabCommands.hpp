#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>

namespace Stand
{
	class CommandCommandTextScale : public CommandSlider
	{
	public:
		explicit CommandCommandTextScale(CommandList* const parent)
			: CommandSlider(parent, LIT("Scale"), CMDNAMES("commandtextscale"),
				NOLABEL, 1, SHRT_MAX, static_cast<int>(Rendering::Theme::kTextScale * 10000.f), 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandTextScale = value / 10000.f;
		}
	};

	class CommandCommandTextXOffset : public CommandSlider
	{
	public:
		explicit CommandCommandTextXOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("X Offset"), CMDNAMES("commandtextxoffset"),
				NOLABEL, -15, 15, 5, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandTextXOffset = static_cast<float>(value);
		}
	};

	class CommandCommandTextYOffset : public CommandSlider
	{
	public:
		explicit CommandCommandTextYOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("Y Offset"), CMDNAMES("commandtextyoffset"),
				NOLABEL, -10, 10, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kCommandTextYOffset = static_cast<float>(value);
		}
	};

	class CommandCommandText : public CommandList
	{
	public:
		CommandCommandTextScale* const scale;
		CommandCommandTextXOffset* const xOffset;
		CommandCommandTextYOffset* const yOffset;

		explicit CommandCommandText(CommandList* const parent)
			: CommandList(parent, LIT("Text"), CMDNAMES("commandtext"))
			, scale(createChild<CommandCommandTextScale>())
			, xOffset(createChild<CommandCommandTextXOffset>())
			, yOffset(createChild<CommandCommandTextYOffset>())
		{
		}
	};

	class CommandPreviewColourInTexture : public CommandToggle
	{
	public:
		explicit CommandPreviewColourInTexture(CommandList* const parent)
			: CommandToggle(parent, LIT("Preview Colour In Texture"), CMDNAMES(),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kPreviewColourInTexture = m_on;
		}
	};

	class CommandPreviewColourInList : public CommandToggle
	{
	public:
		explicit CommandPreviewColourInList(CommandList* const parent)
			: CommandToggle(parent, LIT("Preview Colour In List"), CMDNAMES(),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kPreviewColourInList = m_on;
		}
	};

	class CommandColourCommands : public CommandList
	{
	public:
		CommandPreviewColourInTexture* const previewTexture;
		CommandPreviewColourInList* const previewList;

		explicit CommandColourCommands(CommandList* const parent)
			: CommandList(parent, LIT("Colour Commands"), CMDNAMES())
			, previewTexture(createChild<CommandPreviewColourInTexture>())
			, previewList(createChild<CommandPreviewColourInList>())
		{
		}
	};

	class CommandPrefillSliders : public CommandToggle
	{
	public:
		explicit CommandPrefillSliders(CommandList* const parent)
			: CommandToggle(parent, LIT("Sliders"), CMDNAMES(),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kPrefillSliders = m_on;
		}
	};

	class CommandPrefillTextInputs : public CommandToggle
	{
	public:
		explicit CommandPrefillTextInputs(CommandList* const parent)
			: CommandToggle(parent, LIT("Text Inputs"), CMDNAMES(),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kPrefillTextInputs = m_on;
		}
	};

	class CommandPrefillCommandBox : public CommandList
	{
	public:
		CommandPrefillSliders* const sliders;
		CommandPrefillTextInputs* const textInputs;

		explicit CommandPrefillCommandBox(CommandList* const parent)
			: CommandList(parent, LIT("Prefill Current Value For Command Box"), CMDNAMES())
			, sliders(createChild<CommandPrefillSliders>())
			, textInputs(createChild<CommandPrefillTextInputs>())
		{
		}
	};

	class CommandSelectableNASliders : public CommandToggle
	{
	public:
		explicit CommandSelectableNASliders(CommandList* const parent)
			: CommandToggle(parent, LIT("Selectable N/A Sliders"), CMDNAMES("selectablenasliders"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kSelectableNASliders = m_on;
		}
	};

	class CommandSlidersRightbound : public CommandToggle
	{
	public:
		explicit CommandSlidersRightbound(CommandList* const parent)
			: CommandToggle(parent, LIT("Rightbound Value"), CMDNAMES("slidersrightbound"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kSlidersRightbound = m_on;
		}
	};

	class CommandNumberSliders : public CommandList
	{
	public:
		CommandSelectableNASliders* const selectableNA;
		CommandSlidersRightbound* const rightbound;

		explicit CommandNumberSliders(CommandList* const parent)
			: CommandList(parent, LIT("Number Sliders"), CMDNAMES())
			, selectableNA(createChild<CommandSelectableNASliders>())
			, rightbound(createChild<CommandSlidersRightbound>())
		{
		}
	};

	class CommandSelectableDividers : public CommandToggle
	{
	public:
		explicit CommandSelectableDividers(CommandList* const parent)
			: CommandToggle(parent, LIT("Selectable Dividers"), CMDNAMES("selectabledividers"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kSelectableDividers = m_on;
		}
	};

	class CommandTabCommands : public CommandList
	{
	public:
		CommandCommandText* const text;
		CommandColourCommands* const colourCommands;
		CommandPrefillCommandBox* const prefillCommandBox;
		CommandNumberSliders* const numberSliders;
		CommandSelectableDividers* const selectableDividers;

		explicit CommandTabCommands()
			: CommandList(nullptr, LIT("Commands"), CMDNAMES())
			, text(createChild<CommandCommandText>())
			, colourCommands(createChild<CommandColourCommands>())
			, prefillCommandBox(createChild<CommandPrefillCommandBox>())
			, numberSliders(createChild<CommandNumberSliders>())
			, selectableDividers(createChild<CommandSelectableDividers>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabCommands& GetCommandTabCommands();
}
