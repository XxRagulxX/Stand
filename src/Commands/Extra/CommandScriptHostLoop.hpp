#pragma once

#include "Commands/Widgets/CommandToggleRegular.hpp"

namespace Stand
{
	class CommandScriptHostLoop : public CommandToggleRegular
	{
	public:
		explicit CommandScriptHostLoop(CommandList* parent);

		void onEnableInner(Click& click) final;
	};
}
