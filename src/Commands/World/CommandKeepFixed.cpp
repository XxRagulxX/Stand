#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class KeepFixed : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			auto veh = Self::GetVehicle();
			if (veh)
				veh.Fix();
		}
	};

	static KeepFixed _KeepFixed{"keepfixed", "Keep Vehicle Fixed", "Keeps your vehicle clean and fixed"};
}