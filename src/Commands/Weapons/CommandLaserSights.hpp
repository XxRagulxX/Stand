#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "lib/soup/Vector3.hpp"

namespace Stand
{
	class CommandLaserSights : public CommandToggle
	{
	private:
		soup::Vector3 weapon_pos;

	public:
		explicit CommandLaserSights(CommandList* const parent);

		void onEnable(Click& click) final;
	};
}
