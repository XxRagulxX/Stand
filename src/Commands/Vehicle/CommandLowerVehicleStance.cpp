#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class LowerVehicleStance : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			auto veh = Self::GetVehicle();
			if (veh)
			{
				veh.LowerStance(true);
			}
		}

		virtual void OnDisable() override
		{
			auto veh = Self::GetVehicle();
			if (veh)
			{
				veh.LowerStance(false);
			}
		}
	};

	static LowerVehicleStance _LowerVehicleStance{"lowervehiclestance", "Lower Vehicle Stance", "Lowers your current vehicle's stance. Only some vehicles are supported"};
}