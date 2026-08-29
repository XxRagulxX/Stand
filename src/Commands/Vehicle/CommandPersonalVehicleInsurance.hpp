#pragma once

#if false
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandPersonalVehicleInsurance : public CommandToggle
	{
	public:
		explicit CommandPersonalVehicleInsurance(CommandList* parent);

		void onChange(Click& click) final;
		
		void onTickInGameViewport() final;

		void onTickInWebViewport() final;
	};
}
#endif
