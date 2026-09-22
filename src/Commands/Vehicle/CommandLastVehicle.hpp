#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListLastVehicle : public CommandList
	{
	public:
		explicit CommandListLastVehicle(CommandList* parent);
	};
}
