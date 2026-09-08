#include "Commands/LoopedCommand.hpp"
#include "Commands/CommandListSelect.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	static std::vector<std::pair<int, const char*>> g_BoostBehavior = {
	    {0, "Infinite"},
	    {1, "Instant recharge"}};

	static CommandListSelect _BoostBehavior{"boostbehavior", "Boost Behavior", "Desired rocket boost behavior", g_BoostBehavior, 0};

	class ModifyRocketBoost : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto vehicle = Self::GetVehicle())
			{
				if (vehicle.SupportsBoost())
				{
					auto desiredBehavior = _BoostBehavior.GetState();

					/* Infinite */
					if (desiredBehavior == 0)
						vehicle.SetBoostCharge();

					/* Instant charge */
					else if (desiredBehavior == 1)
						if (!vehicle.IsBoostActive())
							vehicle.SetBoostCharge();
				}
			}
		}
	};

	static ModifyRocketBoost _ModifyRocketBoost{"modifyboostbehavior", "Modify Boost Behavior", "Modify rocket boost behavior for vehicles that support it"};
};