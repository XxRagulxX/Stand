#pragma once

#include "Commands/Stand/CommandListWarning.hpp"

namespace Stand
{
	class CommandListProtections : public CommandListWarning
	{
	public:
		explicit CommandListProtections(CommandList* const parent);

	protected:
		bool shouldShowWarning() const final;
	};
}
