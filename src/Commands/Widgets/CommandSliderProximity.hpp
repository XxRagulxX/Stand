#pragma once

#include "Commands/Widgets/CommandWithOnTickFocused.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"

#include "Rendering/DrawUtil3d.hpp"
#include "Game/natives.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandSliderProximity : public CommandWithOnTickFocused<CommandSliderFloat>
	{
	public:
		explicit CommandSliderProximity(CommandList* const parent, std::vector<CommandName>&& command_names, int default_value = 1000)
			: CommandWithOnTickFocused(parent, LOC("PRX"), std::move(command_names), NOLABEL, 0, 100000, default_value, 100)
		{
		}

		void onTickFocused() final
		{
			if (!g_renderer.stream_proof)
			{
				GRAPHICS::SET_BACKFACECULLING(FALSE);
				DrawUtil3d::draw_sphere(g_player_ent.getPos(), getFloatValue(), (int)(g_renderer.arColour.x * 255.0f), (int)(g_renderer.arColour.y * 255.0f), (int)(g_renderer.arColour.z * 255.0f), 127);
			}
		}
	};
}
