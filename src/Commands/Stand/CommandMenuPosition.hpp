#pragma once

#include "Rendering/StandPort/CommandPosition2d.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandMenuPosition : public CommandPosition2d
	{
	public:
		explicit CommandMenuPosition(CommandList* parent)
			: CommandPosition2d(parent, LOC("MNUPOS"), CMDNAME("menu"),
				{Rendering::Theme::kDefaultMenuOriginX, Rendering::Theme::kDefaultMenuOriginY})
		{
		}

		void onChange(Rendering::Position2d&& pos) const final
		{
			Rendering::Theme::kMenuOriginX = pos.x;
			Rendering::Theme::kMenuOriginY = pos.y;
		}
	};
}
