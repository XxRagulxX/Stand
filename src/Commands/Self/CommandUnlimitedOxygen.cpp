#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class UnlimitedOxygen : LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			Self::GetPed().SetMaxTimeUnderwater(INT_MAX);
		}

		virtual void OnDisable() override
		{
			Self::GetPed().SetMaxTimeUnderwater(-1.0f); // default value
		}
	};

	static UnlimitedOxygen _UnlimitedOxygen{"unlimitedoxygen", "Unlimited Oxygen", "Allows you to stay underwater without losing oxygen."};
}