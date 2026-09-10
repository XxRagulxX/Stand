#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <windows.h>

namespace Stand
{
	class CommandMenuX : public CommandSlider
	{
	public:
		explicit CommandMenuX(CommandList* const parent)
			: CommandSlider(parent, LIT("X"), CMDNAMES("menux"),
				LIT("The menu's own horizontal position."),
				SHRT_MIN, SHRT_MAX, Rendering::Theme::kDefaultMenuOriginX, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kMenuOriginX = static_cast<int16_t>(value);
		}
	};

	class CommandMenuY : public CommandSlider
	{
	public:
		explicit CommandMenuY(CommandList* const parent)
			: CommandSlider(parent, LIT("Y"), CMDNAMES("menuy"),
				LIT("The menu's own vertical position."),
				SHRT_MIN, SHRT_MAX, Rendering::Theme::kDefaultMenuOriginY, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kMenuOriginY = static_cast<int16_t>(value);
		}
	};

	class CommandMenuMouseMove : public CommandToggle
	{
	public:
		explicit CommandMenuMouseMove(CommandList* const parent, CommandMenuX* x, CommandMenuY* y)
			: CommandToggle(parent, LIT("Move With Mouse"), CMDNAMES("menumousemove"),
				LIT("While on, moving the mouse drags the whole menu with it - turn this back off to drop it in place."),
				false)
			, m_X(x)
			, m_Y(y)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandMenuMouseMove()
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!m_on)
			{
				m_Dragging = false;
				return;
			}

			POINT cur{};
			GetCursorPos(&cur);

			if (!m_Dragging)
			{
				m_Dragging = true;
				m_StartCursor = cur;
				m_StartOriginX = Rendering::Theme::kMenuOriginX;
				m_StartOriginY = Rendering::Theme::kMenuOriginY;
				return;
			}

			const auto scale = GetHSpaceScale();
			const auto newX = static_cast<int>(m_StartOriginX + static_cast<float>(cur.x - m_StartCursor.x) / scale);
			const auto newY = static_cast<int>(m_StartOriginY + static_cast<float>(cur.y - m_StartCursor.y) / scale);

			if (newX != m_X->value)
			{
				Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
				m_X->setValue(click, newX);
			}
			if (newY != m_Y->value)
			{
				Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
				m_Y->setValue(click, newY);
			}
		}

	private:
		CommandMenuX* const m_X;
		CommandMenuY* const m_Y;
		bool m_Dragging = false;
		POINT m_StartCursor{};
		int16_t m_StartOriginX = 0;
		int16_t m_StartOriginY = 0;

		static float GetHSpaceScale()
		{
			const auto resX = static_cast<float>(*Pointers.ScreenResX);
			const auto resY = static_cast<float>(*Pointers.ScreenResY);
			if (resX <= 0.f || resY <= 0.f)
				return 1.f;
			return std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight);
		}
	};

	class CommandTabPosition : public CommandList
	{
	public:
		CommandMenuX* const x;
		CommandMenuY* const y;
		CommandMenuMouseMove* const mouseMove;

		explicit CommandTabPosition()
			: CommandList(nullptr, LIT("Position"), CMDNAMES("menupos"))
			, x(createChild<CommandMenuX>())
			, y(createChild<CommandMenuY>())
			, mouseMove(createChild<CommandMenuMouseMove>(x, y))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabPosition& GetCommandTabPosition();
}
