#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class InfiniteParachutes : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			PLAYER::SET_PLAYER_HAS_RESERVE_PARACHUTE(Self::GetPlayer().GetId());

			if (auto ped = Self::GetPed())
			{
				if (!ped.HasWeapon("GADGET_PARACHUTE"_J))
					ped.GiveWeapon("GADGET_PARACHUTE"_J);
			}
		}
	};

	static InfiniteParachutes _InfiniteParachutes{"infiniteparachutes", "Infinite Parachutes", "Always have a parachute"};
};