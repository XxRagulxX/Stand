#pragma once
#include "Commands/Settings/CommandTabAddressBar.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <string>

namespace Stand
{
	class CommandTabsWidth : public CommandSlider
	{
	public:
		explicit CommandTabsWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("tabswidth"),
				NOLABEL, 1, SHRT_MAX, Rendering::Theme::kSidebarWidth, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabsWidth = static_cast<int16_t>(value);
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandTabsHeight : public CommandSlider
	{
	public:
		explicit CommandTabsHeight(CommandList* const parent)
			: CommandSlider(parent, LIT("Height"), CMDNAMES("tabsheight"),
				NOLABEL, 1, SHRT_MAX, Rendering::Theme::kSidebarEntryHeight, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabsHeight = static_cast<int16_t>(value);
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandTabsTextScale : public CommandSlider
	{
	public:
		explicit CommandTabsTextScale(CommandList* const parent)
			: CommandSlider(parent, LIT("Scale"), CMDNAMES("tabstextscale"),
				NOLABEL, 1, 32767, static_cast<int>(Rendering::Theme::kTextScale * 10000.f), 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabsTextScale = value / 10000.f;
		}
	};

	class CommandTabsTextXOffset : public CommandSlider
	{
	public:
		explicit CommandTabsTextXOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("X Offset"), CMDNAMES("tabstextxoffset"),
				NOLABEL, -15, 15, 5, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabsTextXOffset = static_cast<float>(value);
		}
	};

	class CommandTabsTextYOffset : public CommandSlider
	{
	public:
		explicit CommandTabsTextYOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("Y Offset"), CMDNAMES("tabstextyoffset"),
				NOLABEL, -10, 10, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabsTextYOffset = static_cast<float>(value);
		}
	};

	class CommandTabsTextList : public CommandList
	{
	public:
		CommandTabsTextScale* const scale;
		CommandTabsTextXOffset* const xOffset;
		CommandTabsTextYOffset* const yOffset;

		explicit CommandTabsTextList(CommandList* const parent)
			: CommandList(parent, LIT("Text"), CMDNAMES("tabstext"))
			, scale(createChild<CommandTabsTextScale>())
			, xOffset(createChild<CommandTabsTextXOffset>())
			, yOffset(createChild<CommandTabsTextYOffset>())
		{
		}
	};

	class CommandTabsAlignment : public CommandSlider
	{
		static constexpr const char* kLabels[3] = {"Left", "Centre", "Right"};

	public:
		explicit CommandTabsAlignment(CommandList* const parent)
			: CommandSlider(parent, LIT("Text Alignment"), CMDNAMES("tabsalignment"),
				NOLABEL, 0, 2, 0, 1)
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
			case 1:
				Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Centre;
				break;
			case 2:
				Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Right;
				break;
			default:
				Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Left;
				break;
			}
		}
	};

	class CommandTabsLeftIcon : public CommandToggle
	{
	public:
		explicit CommandTabsLeftIcon(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Icon Left"), CMDNAMES("tabslefticon"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsShowLeftIcon = m_on;
		}
	};

	class CommandTabsShowName : public CommandToggle
	{
	public:
		explicit CommandTabsShowName(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Name"), CMDNAMES("tabsname"),
				NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsShowName = m_on;
		}
	};

	class CommandTabsRightIcon : public CommandToggle
	{
	public:
		explicit CommandTabsRightIcon(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Icon Right"), CMDNAMES("tabsrighticon"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsShowRightIcon = m_on;
		}
	};

	class CommandTabsIconsTextColour : public CommandToggle
	{
	public:
		explicit CommandTabsIconsTextColour(CommandList* const parent)
			: CommandToggle(parent, LIT("Icons Use Text Colour"), CMDNAMES(),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsIconsUseTextColour = m_on;
		}
	};

	class CommandTabTabs : public CommandList
	{
	public:
		CommandTabsWidth* const width;
		CommandTabsHeight* const height;
		CommandTabsTextList* const text;
		CommandTabsAlignment* const alignment;
		CommandTabsLeftIcon* const leftIcon;
		CommandTabsShowName* const showName;
		CommandTabsRightIcon* const rightIcon;
		CommandTabsIconsTextColour* const iconsTextColour;

		explicit CommandTabTabs()
			: CommandList(nullptr, LIT("Tabs"), CMDNAMES())
			, width(createChild<CommandTabsWidth>())
			, height(createChild<CommandTabsHeight>())
			, text(createChild<CommandTabsTextList>())
			, alignment(createChild<CommandTabsAlignment>())
			, leftIcon(createChild<CommandTabsLeftIcon>())
			, showName(createChild<CommandTabsShowName>())
			, rightIcon(createChild<CommandTabsRightIcon>())
			, iconsTextColour(createChild<CommandTabsIconsTextColour>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabTabs& GetCommandTabTabs();
}
