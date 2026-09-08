#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandTennisMode - MISC::ENABLE_
	// TENNIS_MODE only needs calling when the game's own state disagrees
	// with what this toggle wants, same as Stand's own version.
	class TennisMode : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			const auto handle = Self::GetPed().GetHandle();
			const bool on = GetState();
			if (static_cast<bool>(MISC::IS_TENNIS_MODE(handle)) != on)
				MISC::ENABLE_TENNIS_MODE(handle, on, on);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			MISC::ENABLE_TENNIS_MODE(Self::GetPed().GetHandle(), false, false);
		}
	};

	static TennisMode _TennisMode{"tennismode", "Tennis Mode", "Sprint like you're playing tennis"};
}
