#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <windows.h>

namespace Stand
{
	class CommandNotifyX : public CommandSlider
	{
	public:
		explicit CommandNotifyX(CommandList* const parent)
			: CommandSlider(parent, LIT("X"), CMDNAMES("notifyx"),
				LIT("Horizontal position of the notification panel."),
				SHRT_MIN, SHRT_MAX, 0, 10)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kCustomPositionX = static_cast<int16_t>(value);
		}
	};

	class CommandNotifyY : public CommandSlider
	{
	public:
		explicit CommandNotifyY(CommandList* const parent)
			: CommandSlider(parent, LIT("Y"), CMDNAMES("notifyy"),
				LIT("Vertical position of the notification panel."),
				SHRT_MIN, SHRT_MAX, 0, 10)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kCustomPositionY = static_cast<int16_t>(value);
		}
	};

	class CommandNotifyMouseMove : public CommandToggle
	{
	public:
		explicit CommandNotifyMouseMove(CommandList* const parent, CommandNotifyX* x, CommandNotifyY* y)
			: CommandToggle(parent, LIT("Move With Mouse"), CMDNAMES("notifymousemove"),
				LIT("While on, moving the mouse repositions the notification panel."),
				false)
			, m_X(x)
			, m_Y(y)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandNotifyMouseMove()
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
				m_StartX = static_cast<int>(Rendering::NotifySettings::kCustomPositionX);
				m_StartY = static_cast<int>(Rendering::NotifySettings::kCustomPositionY);
				return;
			}

			const int newX = m_StartX + (cur.x - m_StartCursor.x);
			const int newY = m_StartY + (cur.y - m_StartCursor.y);

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
		CommandNotifyX* const m_X;
		CommandNotifyY* const m_Y;
		bool m_Dragging = false;
		POINT m_StartCursor{};
		int m_StartX = 0;
		int m_StartY = 0;
	};

	class CommandTabNotifyPosition : public CommandList
	{
	public:
		CommandNotifyX* const x;
		CommandNotifyY* const y;
		CommandNotifyMouseMove* const mouseMove;

		explicit CommandTabNotifyPosition()
			: CommandList(nullptr, LIT("Notify Position"))
			, x(createChild<CommandNotifyX>())
			, y(createChild<CommandNotifyY>())
			, mouseMove(createChild<CommandNotifyMouseMove>(x, y))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabNotifyPosition& GetCommandTabNotifyPosition();
}
