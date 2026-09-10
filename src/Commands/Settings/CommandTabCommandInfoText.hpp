#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <string>

namespace Stand
{
	class CommandInfoTextPosition : public CommandSlider
	{
		static constexpr const char* kLabels[4] = {"Left", "Bottom", "Right", "Below Tabs"};

	public:
		explicit CommandInfoTextPosition(CommandList* const parent)
			: CommandSlider(parent, LIT("Position"), CMDNAMES("commandinfotextposition"),
				NOLABEL, 0, 3, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return kLabels[std::clamp(value, 0, 3)];
		}

		void onChange(Click& click, int prev_value) override
		{
			switch (value)
			{
			case 1:
				Rendering::Theme::kInfoTextPosition = Rendering::Theme::InfoTextPosition::Bottom;
				break;
			case 2:
				Rendering::Theme::kInfoTextPosition = Rendering::Theme::InfoTextPosition::Right;
				break;
			case 3:
				Rendering::Theme::kInfoTextPosition = Rendering::Theme::InfoTextPosition::BelowTabs;
				break;
			default:
				Rendering::Theme::kInfoTextPosition = Rendering::Theme::InfoTextPosition::Left;
				break;
			}
		}
	};

	class CommandInfoWidth : public CommandSlider
	{
	public:
		explicit CommandInfoWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("infowidth"),
				NOLABEL, 1, SHRT_MAX, 300, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kInfoWidth = static_cast<int16_t>(value);
		}
	};

	class CommandInfoPadding : public CommandSlider
	{
	public:
		explicit CommandInfoPadding(CommandList* const parent)
			: CommandSlider(parent, LIT("Padding"), CMDNAMES("infopadding"),
				NOLABEL, -100, 100, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kInfoPadding = static_cast<int16_t>(value);
		}
	};

	class CommandShowHelpText : public CommandToggle
	{
	public:
		explicit CommandShowHelpText(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Help Text"), CMDNAMES("showhelptext"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowHelpText = m_on;
		}
	};

	class CommandShowSyntax : public CommandToggle
	{
	public:
		explicit CommandShowSyntax(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Command Syntax"), CMDNAMES("showsyntax"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowSyntax = m_on;
		}
	};

	class CommandShowSliderBehaviour : public CommandToggle
	{
	public:
		explicit CommandShowSliderBehaviour(CommandList* const parent)
			: CommandToggle(parent, LIT("Indicate Slider Behaviour"), CMDNAMES("showsliderbehaviour"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowSliderBehaviour = m_on;
		}
	};

	class CommandShowNonUserOnly : public CommandToggle
	{
	public:
		explicit CommandShowNonUserOnly(CommandList* const parent)
			: CommandToggle(parent, LIT("Indicate If Usable By Other Players"), CMDNAMES("shownonuseronly"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowNonUserOnly = m_on;
		}
	};

	class CommandShowOwners : public CommandToggle
	{
	public:
		explicit CommandShowOwners(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Owning Script"), CMDNAMES("showowners"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowOwners = m_on;
		}
	};

	class CommandTabCommandInfoText : public CommandList
	{
	public:
		CommandInfoTextPosition* const position;
		CommandInfoWidth* const width;
		CommandInfoPadding* const padding;
		CommandShowHelpText* const showHelpText;
		CommandShowSyntax* const showSyntax;
		CommandShowSliderBehaviour* const showSliderBehaviour;
		CommandShowNonUserOnly* const showNonUserOnly;
		CommandShowOwners* const showOwners;

		explicit CommandTabCommandInfoText()
			: CommandList(nullptr, LIT("Command Info Text"), CMDNAMES())
			, position(createChild<CommandInfoTextPosition>())
			, width(createChild<CommandInfoWidth>())
			, padding(createChild<CommandInfoPadding>())
			, showHelpText(createChild<CommandShowHelpText>())
			, showSyntax(createChild<CommandShowSyntax>())
			, showSliderBehaviour(createChild<CommandShowSliderBehaviour>())
			, showNonUserOnly(createChild<CommandShowNonUserOnly>())
			, showOwners(createChild<CommandShowOwners>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabCommandInfoText& GetCommandTabCommandInfoText();
}
