#pragma once

#include "Rendering/StandPort/CommandPosition2d.hpp"

namespace Stand
{
	class CommandInfoPos : public CommandPosition2d
	{
	public:
		explicit CommandInfoPos(CommandList* parent)
			: CommandPosition2d(parent, LOC("POS"), CMDNAME("infotext"), { 1886, 98 })
		{
		}

		void onActiveListUpdate() final
		{
		}

		void onChange(Rendering::Position2d&& pos) const final
		{
		}
	};
}
