#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "lib/soup/Bytepatch.hpp"

namespace Stand
{
	class CommandDisableInVehicleHairScale : public CommandToggle
	{
	private:
		soup::Bytepatch m_patch;

	public:
		explicit CommandDisableInVehicleHairScale(CommandList* const parent);

		void onEnable(Click& click) final;
		void onDisable(Click& click) final;
	};
}
