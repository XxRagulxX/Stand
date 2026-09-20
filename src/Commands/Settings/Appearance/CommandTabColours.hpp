#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/Theme.hpp"
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
