#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/dx_common.hpp"
#include "Menu/MouseMode.hpp"
#include "Util/Position2d.hpp"

namespace Stand
{
	class CommandPosition2dMouse : public CommandToggle
	{
	private:
		Position2d parent_start_pos{};
		DirectX::SimpleMath::Vector2 cursor_start_pos;
		MouseMode prev_mouse_mode;

	public:
		explicit CommandPosition2dMouse(CommandList* parent, const CommandName& command_name);

		void onEnable(Click& click) final;
	private:
		void updatePos(DirectX::SimpleMath::Vector2&& cursor_pos) const;
	public:
		void onDisable(Click& click) final;
	};
}
