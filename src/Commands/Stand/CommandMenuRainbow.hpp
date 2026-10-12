#pragma once

#include "Commands/Widgets/CommandColourRainbow.hpp"

namespace Stand
{
	class CommandMenuRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandMenuRainbow(CommandList* const parent);

	protected:
		void onHueChanged() final;
	};
}
