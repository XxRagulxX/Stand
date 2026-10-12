#pragma once
#include "Commands/Widgets/CommandColourRainbow.hpp"
#include "Rendering/StandPort/CommandColour.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"
#include "Util/get_current_time_millis.hpp"

#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

namespace Stand
{

	class CommandRainbowMode : public CommandColourRainbow
	{
	public:
		explicit CommandRainbowMode(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("rainbow"), Rendering::Theme::kAccent)
		{}
	};

	class CommandHudRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandHudRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("hudrainbow"), Rendering::Theme::kHud)
		{}
	};

	class CommandArRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandArRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("arrainbow"), Rendering::Theme::kAr)
		{}
	};

	class CommandMinigameRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandMinigameRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("minigamerainbow"), Rendering::Theme::kMinigame)
		{}
	};

	class CommandPrimaryColour : public CommandColour
	{
	public:
		explicit CommandPrimaryColour(CommandList* const parent)
			: CommandColour(parent, LIT("Primary Colour"), CMDNAMES("primary"),
				LIT("The one accent colour used everywhere in this menu - a sidebar/tab's active entry, a toggle's ON state, a button's fill."),
				255, 0, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kAccent = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandBackgroundColour : public CommandColour
	{
	public:
		explicit CommandBackgroundColour(CommandList* const parent)
			: CommandColour(parent, LIT("Background Colour"), CMDNAMES("background"),
				LIT("The translucent panel background every non-focused row sits on."),
				0, 0, 0, 77)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kPanelBackground = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandFocusTextColour : public CommandColour
	{
	public:
		explicit CommandFocusTextColour(CommandList* const parent)
			: CommandColour(parent, LIT("Focused Text Colour"), CMDNAMES("focustext"),
				LIT("Colour of text in the focused/selected menu row."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kFocusText = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandFocusRightTextColour : public CommandColour
	{
	public:
		explicit CommandFocusRightTextColour(CommandList* const parent)
			: CommandColour(parent, LIT("Focused Right-Bound Text Colour"), CMDNAMES("focusrighttext"),
				LIT("Colour of right-aligned text (values, arrows) in the focused/selected menu row."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kFocusRightText = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandFocusTextureColour : public CommandColour
	{
	public:
		explicit CommandFocusTextureColour(CommandList* const parent)
			: CommandColour(parent, LIT("Focused Texture Colour"), CMDNAMES("focustexture"),
				LIT("Colour of texture/sprite elements (toggle indicator border) in the focused/selected menu row."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kFocusTexture = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandUnfocusedTextColour : public CommandColour
	{
	public:
		explicit CommandUnfocusedTextColour(CommandList* const parent)
			: CommandColour(parent, LIT("Unfocused Text Colour"), CMDNAMES("unfocusedtext"),
				LIT("Colour of text in unfocused/non-selected menu rows."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kUnfocusedText = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandUnfocusedRightTextColour : public CommandColour
	{
	public:
		explicit CommandUnfocusedRightTextColour(CommandList* const parent)
			: CommandColour(parent, LIT("Unfocused Right-Bound Text Colour"), CMDNAMES("unfocusedrighttext"),
				LIT("Colour of right-aligned text (values, arrows) in unfocused/non-selected menu rows."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kUnfocusedRightText = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandUnfocusedTextureColour : public CommandColour
	{
	public:
		explicit CommandUnfocusedTextureColour(CommandList* const parent)
			: CommandColour(parent, LIT("Unfocused Texture Colour"), CMDNAMES("unfocusedtexture"),
				LIT("Colour of texture/sprite elements (toggle indicator border) in unfocused/non-selected menu rows."),
				255, 255, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kUnfocusedTexture = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandHudColour : public CommandColour
	{
	public:
		explicit CommandHudColour(CommandList* const parent)
			: CommandColour(parent, LIT("HUD Colour"), CMDNAMES("hud"),
				LIT("Colour used for HUD overlay elements."),
				255, 0, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kHud = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandArColour : public CommandColour
	{
	public:
		explicit CommandArColour(CommandList* const parent)
			: CommandColour(parent, LIT("AR Colour"), CMDNAMES("ar"),
				LIT("Colour used for augmented reality overlay elements."),
				255, 0, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kAr = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandMinigameColour : public CommandColour
	{
	public:
		explicit CommandMinigameColour(CommandList* const parent)
			: CommandColour(parent, LIT("Minigame Colour"), CMDNAMES("minigame"),
				LIT("Colour used for minigame overlay elements."),
				255, 0, 255, 255)
		{
		}

		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kMinigame = {rgba.x, rgba.y, rgba.z, rgba.w};
		}
	};

	class CommandCopyFocusTextToRightText : public CommandPhysical
	{
	public:
		explicit CommandCopyFocusTextToRightText(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Focused Text Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyFocusTextToTexture : public CommandPhysical
	{
	public:
		explicit CommandCopyFocusTextToTexture(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Focused Text Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyUnfocusedTextToRightText : public CommandPhysical
	{
	public:
		explicit CommandCopyUnfocusedTextToRightText(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Unfocused Text Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyUnfocusedTextToTexture : public CommandPhysical
	{
	public:
		explicit CommandCopyUnfocusedTextToTexture(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Unfocused Text Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyPrimaryToHud : public CommandPhysical
	{
	public:
		explicit CommandCopyPrimaryToHud(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyPrimaryToAr : public CommandPhysical
	{
	public:
		explicit CommandCopyPrimaryToAr(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandCopyPrimaryToMinigame : public CommandPhysical
	{
	public:
		explicit CommandCopyPrimaryToMinigame(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), {}, NOLABEL)
		{}
		void onClick(Click& click) override;
	};

	class CommandTabColours : public CommandList
	{
	public:
		CommandColour* const primary;
		CommandColour* const focusText;
		CommandColour* const focusRightText;
		CommandColour* const focusTexture;
		CommandColour* const background;
		CommandColour* const unfocusedText;
		CommandColour* const unfocusedRightText;
		CommandColour* const unfocusedTexture;
		CommandColour* const hud;
		CommandColour* const ar;
		CommandColour* const minigame;
		CommandRainbowMode* const rainbow;
		CommandCopyFocusTextToRightText* const copyFocusTextToRightText;
		CommandCopyFocusTextToTexture* const copyFocusTextToTexture;
		CommandCopyUnfocusedTextToRightText* const copyUnfocusedTextToRightText;
		CommandCopyUnfocusedTextToTexture* const copyUnfocusedTextToTexture;
		CommandHudRainbow* const hudRainbow;
		CommandCopyPrimaryToHud* const copyPrimaryToHud;
		CommandArRainbow* const arRainbow;
		CommandCopyPrimaryToAr* const copyPrimaryToAr;
		CommandMinigameRainbow* const minigameRainbow;
		CommandCopyPrimaryToMinigame* const copyPrimaryToMinigame;

		explicit CommandTabColours(CommandList* parent = nullptr)
			: CommandList(parent, LIT("Colours"))
			, primary(createChild<CommandPrimaryColour>())
			, focusText(createChild<CommandFocusTextColour>())
			, focusRightText(createChild<CommandFocusRightTextColour>())
			, focusTexture(createChild<CommandFocusTextureColour>())
			, background(createChild<CommandBackgroundColour>())
			, unfocusedText(createChild<CommandUnfocusedTextColour>())
			, unfocusedRightText(createChild<CommandUnfocusedRightTextColour>())
			, unfocusedTexture(createChild<CommandUnfocusedTextureColour>())
			, hud(createChild<CommandHudColour>())
			, ar(createChild<CommandArColour>())
			, minigame(createChild<CommandMinigameColour>())
			, rainbow(createChild<CommandRainbowMode>())
			, copyFocusTextToRightText(createChild<CommandCopyFocusTextToRightText>())
			, copyFocusTextToTexture(createChild<CommandCopyFocusTextToTexture>())
			, copyUnfocusedTextToRightText(createChild<CommandCopyUnfocusedTextToRightText>())
			, copyUnfocusedTextToTexture(createChild<CommandCopyUnfocusedTextToTexture>())
			, hudRainbow(createChild<CommandHudRainbow>())
			, copyPrimaryToHud(createChild<CommandCopyPrimaryToHud>())
			, arRainbow(createChild<CommandArRainbow>())
			, copyPrimaryToAr(createChild<CommandCopyPrimaryToAr>())
			, minigameRainbow(createChild<CommandMinigameRainbow>())
			, copyPrimaryToMinigame(createChild<CommandCopyPrimaryToMinigame>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabColours& GetCommandTabColours();
}
