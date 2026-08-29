#pragma once

#include "Commands/Widgets/CommandReadonlyValue.hpp"

namespace Stand
{
	class CommandHistoryCopy : public CommandReadonlyValue
	{
	public:
		explicit CommandHistoryCopy(CommandList* parent);
		
		void onClick(Click& click) final;

		void onTickInGameViewport() final;
		void onTickInWebViewport() final;
	};
}
