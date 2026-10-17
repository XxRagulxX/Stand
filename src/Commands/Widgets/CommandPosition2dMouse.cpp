#include "Commands/Widgets/CommandPosition2dMouse.hpp"

#include "Rendering/StandPort/CommandPosition2d.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <algorithm>
#include <windows.h>

namespace Stand
{
	CommandPosition2dMouse::CommandPosition2dMouse(CommandList* parent, const CommandName& command_name)
		: CommandToggle(parent, LOC("MOVWTHMSE"), { std::move(CommandName(command_name).append(CMDNAME("mousemove"))) }, NOLABEL, false, CMDFLAGS_TOGGLE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
	{
	}

	void CommandPosition2dMouse::onEnable(Click& click)
	{
		prev_mouse_mode = g_gui.mouse_mode;
		g_gui.mouse_mode = MouseMode::NAVIGATE;

		auto* pos2d = static_cast<CommandPosition2d*>(parent);
		parent_start_pos = pos2d->getPosition();

		POINT pt{};
		GetCursorPos(&pt);
		cursor_start_pos = DirectX::SimpleMath::Vector2(static_cast<float>(pt.x), static_cast<float>(pt.y));

		CommandTickDispatch::AddCommand(this);
	}

	void CommandPosition2dMouse::updatePos(DirectX::SimpleMath::Vector2&& cursor_pos) const
	{
		const float resX = static_cast<float>(*Pointers.ScreenResX);
		const float resY = static_cast<float>(*Pointers.ScreenResY);
		const float scale = (resX > 0.f && resY > 0.f)
			? std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight)
			: 1.f;

		const auto newX = static_cast<int>(parent_start_pos.x + (cursor_pos.x - cursor_start_pos.x) / scale);
		const auto newY = static_cast<int>(parent_start_pos.y + (cursor_pos.y - cursor_start_pos.y) / scale);

		auto* pos2d = static_cast<const CommandPosition2d*>(parent);
		if (newX != pos2d->x->value)
		{
			Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
			const_cast<CommandSlider*>(pos2d->x)->setValue(click, newX);
		}
		if (newY != pos2d->y->value)
		{
			Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
			const_cast<CommandSlider*>(pos2d->y)->setValue(click, newY);
		}
	}

	void CommandPosition2dMouse::onTick()
	{
		if (!m_on)
			return;
		POINT pt{};
		GetCursorPos(&pt);
		updatePos({ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	void CommandPosition2dMouse::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
		g_gui.mouse_mode = prev_mouse_mode;
	}
}
