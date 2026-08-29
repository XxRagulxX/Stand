#pragma once

#include "Commands/Player/CommandPlayerEsp.hpp"
#include "Commands/Widgets/CommandEspBase.hpp"

#include "Core/AbstractEntity.hpp"
#include "Rendering/DrawUtil3d.hpp"

namespace Stand
{
	class CommandPlayerLineEsp : public CommandEspBase<CommandPlayerEsp>
	{
	public:
		explicit CommandPlayerLineEsp(CommandList* const parent, CommandSlider* max_distance, CommandColour* const colour, CommandEspTags* const tag_colours)
			: CommandEspBase(parent, LOC("LINESP"), CMDNAMES("lineesp", "linesp", "playerlineesp", "playerlinesp"), max_distance)
		{
			this->colour = colour;
			this->tag_colours = tag_colours;
		}

		void drawEsp(AbstractPlayer&& p, v3&& pos, float&&) const final
		{
			DrawUtil3d::draw_line(g_player_ped.getPos(), pos, getColour(p));
		}
	};
}
