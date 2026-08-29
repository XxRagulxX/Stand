#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandObjectSpawner : public CommandList
	{
	public:
		size_t objects_offset;

		explicit CommandObjectSpawner(CommandList* parent);
	};
}
