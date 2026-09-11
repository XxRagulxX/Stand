#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class Friction : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		void OnTick() override
		{
			if (auto ped = Self::GetPed())
				PED::SET_PED_MIN_MOVE_BLEND_RATIO(ped.GetHandle(), 1.0f);
		}

		void OnDisable() override
		{
			if (auto ped = Self::GetPed())
				PED::SET_PED_MIN_MOVE_BLEND_RATIO(ped.GetHandle(), 0.0f);
		}
	};

	static Friction _Friction{"friction", "Increased Friction",
	    "Reduces the slipperiness when turning with a high walk/run speed."};
}
