#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class InfiniteStamina : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPlayer())
				return;

			PLAYER::RESTORE_PLAYER_STAMINA(Self::GetPlayer().GetId(), 1.0f);
		}
	};

	static InfiniteStamina _InfiniteStamina{"infinitestamina", "Infinite Stamina", "Never run out of breath while sprinting/swimming"};
}
