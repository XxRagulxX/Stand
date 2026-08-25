#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandGallery : public CommandList
	{
	public:
		explicit CommandGallery(CommandList* const parent);	
	};
}
