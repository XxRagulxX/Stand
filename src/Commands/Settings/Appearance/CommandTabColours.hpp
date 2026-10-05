#pragma once
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
	class CommandColourRainbow : public CommandSlider
	{
	public:
		explicit CommandColourRainbow(CommandList* const parent, Label&& name, std::vector<CommandName>&& cmdnames, DirectX::XMFLOAT4& target)
			: CommandSlider(parent, std::move(name), std::move(cmdnames),
				LIT("Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity."),
				0, 1000, 0, 1)
			, m_Target(target)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandColourRainbow()
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (value <= 0)
			{
				m_LastTick = 0;
				m_MsAccumulated = 0;
				return;
			}

			const auto now = get_current_time_millis();
			if (m_LastTick == 0)
			{
				m_LastTick = now;
				return;
			}

			m_MsAccumulated += now - m_LastTick;
			m_LastTick = now;

			const auto stepMs = static_cast<time_t>(value);
			if (m_MsAccumulated < stepMs)
				return;

			const auto steps = m_MsAccumulated / stepMs;
			m_MsAccumulated -= steps * stepMs;

			auto hsv = RgbToHsv(m_Target);
			hsv.x = std::fmod(hsv.x + static_cast<float>(steps), 360.f);
			m_Target = HsvToRgb(hsv);
		}

	private:
		DirectX::XMFLOAT4& m_Target;
		time_t m_LastTick = 0;
		time_t m_MsAccumulated = 0;

		static DirectX::XMFLOAT4 RgbToHsv(const DirectX::XMFLOAT4& rgb)
		{
			const float maxC = std::max({rgb.x, rgb.y, rgb.z});
			const float minC = std::min({rgb.x, rgb.y, rgb.z});
			const float delta = maxC - minC;

			float hue = 0.f;
			if (delta > 0.f)
			{
				if (maxC == rgb.x)
					hue = 60.f * std::fmod((rgb.y - rgb.z) / delta, 6.f);
				else if (maxC == rgb.y)
					hue = 60.f * (((rgb.z - rgb.x) / delta) + 2.f);
				else
					hue = 60.f * (((rgb.x - rgb.y) / delta) + 4.f);

				if (hue < 0.f)
					hue += 360.f;
			}

			const float sat = (maxC <= 0.f) ? 0.f : (delta / maxC);
			return {hue, sat, maxC, rgb.w};
		}

		static DirectX::XMFLOAT4 HsvToRgb(const DirectX::XMFLOAT4& hsv)
		{
			const float c = hsv.z * hsv.y;
			const float x = c * (1.f - std::fabs(std::fmod(hsv.x / 60.f, 2.f) - 1.f));
			const float m = hsv.z - c;

			float r1 = 0.f, g1 = 0.f, b1 = 0.f;
			if (hsv.x < 60.f)       { r1 = c; g1 = x; }
			else if (hsv.x < 120.f) { r1 = x; g1 = c; }
			else if (hsv.x < 180.f) { g1 = c; b1 = x; }
			else if (hsv.x < 240.f) { g1 = x; b1 = c; }
			else if (hsv.x < 300.f) { r1 = x; b1 = c; }
			else                    { r1 = c; b1 = x; }

			return {r1 + m, g1 + m, b1 + m, hsv.w};
		}
	};

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

		explicit CommandTabColours()
			: CommandList(nullptr, LIT("Colours"))
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
