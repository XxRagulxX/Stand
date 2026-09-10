#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <string>

namespace Stand
{
	class CommandShowAddressbar : public CommandToggle
	{
	public:
		explicit CommandShowAddressbar(CommandList* const parent)
			: CommandToggle(parent, LIT("Address Bar"), CMDNAMES("addressbar"),
				LIT("Whether the address bar at the top of the menu is shown."),
				true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kAddressBarVisible = m_on;
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandAddressSeparator : public CommandSlider
	{
		static constexpr const char* kLabels[8] = {
			"Stand > Online", "Stand>Online",
			"Stand / Online", "Stand/Online",
			"Stand : Online", "Stand:Online",
			"Stand \\ Online", "Stand\\Online",
		};
		static constexpr const char* kValues[8] = {
			" > ", ">", " / ", "/", " : ", ":", " \\ ", "\\"
		};

	public:
		explicit CommandAddressSeparator(CommandList* const parent)
			: CommandSlider(parent, LIT("Address Separator"), CMDNAMES(),
				NOLABEL, 0, 7, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return kLabels[std::clamp(value, 0, 7)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kAddressSeparator = kValues[std::clamp(value, 0, 7)];
		}
	};

	class CommandAddressCurrentListOnly : public CommandToggle
	{
	public:
		explicit CommandAddressCurrentListOnly(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Current List Only"), CMDNAMES(),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kAddressCurrentListOnly = m_on;
		}
	};

	class CommandAddressbarWidthAffectedByScrollbar : public CommandToggle
	{
	public:
		explicit CommandAddressbarWidthAffectedByScrollbar(CommandList* const parent)
			: CommandToggle(parent, LIT("Width Affected By Scrollbar"), CMDNAMES(),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kAddressbarWidthAffectedByScrollbar = m_on;
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandAddressbarWidthAffectedByColumns : public CommandToggle
	{
	public:
		explicit CommandAddressbarWidthAffectedByColumns(CommandList* const parent)
			: CommandToggle(parent, LIT("Width Affected By Columns"), CMDNAMES(),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kAddressbarWidthAffectedByColumns = m_on;
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandAddressbarHeight : public CommandSlider
	{
	public:
		explicit CommandAddressbarHeight(CommandList* const parent)
			: CommandSlider(parent, LIT("Height"), CMDNAMES("addressbarheight"),
				NOLABEL, 0, SHRT_MAX, Rendering::Theme::kHeaderHeight, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kAddressBarHeight = static_cast<int16_t>(value);
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandAddressbarTextScale : public CommandSlider
	{
	public:
		explicit CommandAddressbarTextScale(CommandList* const parent)
			: CommandSlider(parent, LIT("Scale"), CMDNAMES("addressbartextscale"),
				NOLABEL, 1, 32767, static_cast<int>(Rendering::Theme::kSmallTextScale * 10000.f), 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kAddressbarTextScale = value / 10000.f;
		}
	};

	class CommandAddressbarTextXOffset : public CommandSlider
	{
	public:
		explicit CommandAddressbarTextXOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("X Offset"), CMDNAMES("addressbartextxoffset"),
				NOLABEL, -15, 15, 5, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kAddressbarTextXOffset = static_cast<float>(value);
		}
	};

	class CommandAddressbarTextYOffset : public CommandSlider
	{
	public:
		explicit CommandAddressbarTextYOffset(CommandList* const parent)
			: CommandSlider(parent, LIT("Y Offset"), CMDNAMES("addressbartextyoffset"),
				NOLABEL, -10, 10, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kAddressbarTextYOffset = static_cast<float>(value);
		}
	};

	class CommandAddressbarText : public CommandList
	{
	public:
		CommandAddressbarTextScale* const scale;
		CommandAddressbarTextXOffset* const xOffset;
		CommandAddressbarTextYOffset* const yOffset;

		explicit CommandAddressbarText(CommandList* const parent)
			: CommandList(parent, LIT("Text"), CMDNAMES("addressbartext"))
			, scale(createChild<CommandAddressbarTextScale>())
			, xOffset(createChild<CommandAddressbarTextXOffset>())
			, yOffset(createChild<CommandAddressbarTextYOffset>())
		{
		}
	};

	class CommandCursorPos : public CommandToggle
	{
	public:
		explicit CommandCursorPos(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Cursor Position"), CMDNAMES("cursorpos"),
				LIT("Toggles the visibility of the cursor position indicator at the top-right of Stand's in-game UI."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kShowCursorPos = m_on;
		}
	};

	class CommandCursorPosIncludesDividers : public CommandToggle
	{
	public:
		explicit CommandCursorPosIncludesDividers(CommandList* const parent)
			: CommandToggle(parent, LIT("Cursor Position Includes Dividers"), CMDNAMES(),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kCursorPosIncludesDividers = m_on;
		}
	};

	class CommandTabAddressBar : public CommandList
	{
	public:
		CommandShowAddressbar* const show;
		CommandAddressSeparator* const separator;
		CommandAddressCurrentListOnly* const currentListOnly;
		CommandAddressbarWidthAffectedByScrollbar* const widthScrollbar;
		CommandAddressbarWidthAffectedByColumns* const widthColumns;
		CommandAddressbarHeight* const height;
		CommandAddressbarText* const text;
		CommandCursorPos* const cursorPos;
		CommandCursorPosIncludesDividers* const cursorPosDividers;

		explicit CommandTabAddressBar()
			: CommandList(nullptr, LIT("Address Bar"), CMDNAMES())
			, show(createChild<CommandShowAddressbar>())
			, separator(createChild<CommandAddressSeparator>())
			, currentListOnly(createChild<CommandAddressCurrentListOnly>())
			, widthScrollbar(createChild<CommandAddressbarWidthAffectedByScrollbar>())
			, widthColumns(createChild<CommandAddressbarWidthAffectedByColumns>())
			, height(createChild<CommandAddressbarHeight>())
			, text(createChild<CommandAddressbarText>())
			, cursorPos(createChild<CommandCursorPos>())
			, cursorPosDividers(createChild<CommandCursorPosIncludesDividers>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabAddressBar& GetCommandTabAddressBar();
}
